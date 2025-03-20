#include "utils.h"
#include "object.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

bool
is_character (char ch)
{
    return isalnum (ch) || ch == '_';
}

bool
is_number (char *literal)
{
    int size = strlen (literal);
    int i;
    for (i = 0; i < size; ++i)
        {
            if (!isdigit (literal[i]))
                {
                    return false;
                }
        }
    return true;
}

uint32_t
hash_string (char *str)
{
    uint32_t hash_value;
    for (hash_value = 0; *str != '\0'; str++)
        hash_value = *str + hash_value * 31;
    return hash_value % HASH_TABLE_SIZE;
}

uint64_t
hash_pointer (uint64_t pointer)
{
    uint64_t hash_value;
    hash_value = ((pointer >> 16) ^ pointer) * 0x45d9f3b;
    hash_value = ((pointer >> 16) ^ pointer) * 0x45d9f3b;
    hash_value = (pointer >> 16) ^ pointer;
    return hash_value % HASH_TABLE_SIZE;
}

generic_linked_list_t *
create_linked_list (linked_list_type type)
{
    generic_linked_list_t *ll
        = (generic_linked_list_t *)calloc (1, sizeof (generic_linked_list_t));
    ll->type = type;
    return ll;
}

generic_hash_table_t *
create_hash_table (hash_key_type key_type, hash_value_type value_type)
{
    generic_hash_table_t *hash_table
        = (generic_hash_table_t *)calloc (1, sizeof (generic_hash_table_t));
    hash_table->key_type = key_type;
    hash_table->value_type = value_type;
    hash_table->occupied_size = 0;
    hash_table->items = (generic_linked_list_t **)calloc (
        HASH_TABLE_SIZE, sizeof (generic_linked_list_t));
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
        {
            hash_table->items[i] = (generic_linked_list_t *)calloc (
                1, sizeof (generic_linked_list_t));
            hash_table->items[i]->type = LL_KEY_VALUE_TYPE;
        }
    return hash_table;
}

uint64_t
hash_hash_key_t (hash_key_t *key)
{
    return hash_pointer (key->value);
}

void
push_linked_list (generic_linked_list_t *ll, void *value)
{

    generic_linked_list_node_t *ll_node
        = (generic_linked_list_node_t *)calloc (
            1, sizeof (generic_linked_list_node_t));
    generic_linked_list_node_t *curr_tail = ll->tail;
    ll_node->type = ll->type;
    ll_node->data = value;
    ll_node->next = NULL;
    if (curr_tail == NULL)
        {
            ll->head = ll_node;
            ll->tail = ll_node;
            return;
        }
    ll->tail->next = ll_node;
    ll->tail = ll_node;
}

uint64_t
generic_hash (hash_key_type type, void *key)
{
    switch (type)
        {
        case HASH_KEY_POINTER_TYPE:
            {
                return hash_pointer ((uint64_t)key) % HASH_TABLE_SIZE;
            }
        case HASH_KEY_HASH_KEY_TYPE:
            {
                return ((hash_key_t *)key)->value % HASH_TABLE_SIZE;
            }
        case HASH_KEY_STRING_TYPE:
            {
                return hash_string ((char *)key) % HASH_TABLE_SIZE;
            }
        default:
            {
                assert (0 && "Unsupported");
            }
        }
}

bool
insert_hash_table (generic_hash_table_t *hash_table, void *key, void *value)
{

    uint64_t key_hash = generic_hash (hash_table->key_type, key);
    generic_linked_list_t *ll = hash_table->items[key_hash];
    generic_key_value_t *key_value
        = (generic_key_value_t *)calloc (1, sizeof (generic_key_value_t));
    key_value->key_type = hash_table->key_type;
    key_value->value_type = hash_table->value_type;
    key_value->key = key;
    key_value->value = value;
    push_linked_list (ll, key_value);
    return true;
}

void
free_ll_node (generic_linked_list_node_t *ll_node)
{
    FREE (ll_node->data);
    FREE (ll_node);
}

void
free_ll (generic_linked_list_t *ll)
{
    generic_linked_list_node_t *runner = ll->head;
    while (runner != NULL)
        {
            free_ll_node (runner->next);
        }
    FREE (runner);
    FREE (ll);
}

void
free_hash_table (generic_hash_table_t *hash_table)
{
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
        {
            free_ll (hash_table->items[i]);
        }
    FREE (hash_table);
}

bool
key_equals (hash_key_type type, void *key1, void *key2)
{
    switch (type)
        {
        case HASH_KEY_POINTER_TYPE:
            {
                return (*((uint64_t *)key1)) == (*((uint64_t *)key1));
            }
        case HASH_KEY_HASH_KEY_TYPE:
            {
                hash_key_t *hash_key1 = (hash_key_t *)key1;
                hash_key_t *hash_key2 = (hash_key_t *)key2;
                return hash_key1->value == hash_key2->value;
            }
        case HASH_KEY_STRING_TYPE:
            {
                char *string_key1 = (char *)key1;
                char *string_key2 = (char *)key2;
                return strcmp (string_key1, string_key2) == 0;
            }
        default:
            {
                assert (0 && "Pain");
            }
        }
}

generic_linked_list_node_t *
generic_linked_list_search (generic_linked_list_t *ll, void *key)
{
    if (ll == NULL || ll->head == NULL)
        {
            return NULL;
        }
    generic_linked_list_node_t *runner = ll->head;
    // TODO: This needs a more black boxish implementation, not doing it for
    // now since the scope of this DS is only for hashmaps
    switch (runner->type)
        {
        case LL_KEY_VALUE_TYPE:
            {
                generic_key_value_t *key_value
                    = (generic_key_value_t *)runner->data;
                while (
                    runner != NULL
                    && !key_equals (key_value->key_type, key_value->key, key))
                    {
                        runner = runner->next;
                    }
                return runner;
            }
        default:
            assert (0 && "Should not hit this");
        }
}

void *
get_value_hash_table (generic_hash_table_t *hash_table, void *key)
{
    uint64_t hash_key = generic_hash (hash_table->key_type, key);
    generic_linked_list_node_t *node
        = generic_linked_list_search (hash_table->items[hash_key], key);
    return node == NULL ? NULL : node->data;
}
