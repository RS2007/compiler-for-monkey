#include "utils.h"
#include "object.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

bool is_character(char ch) { return isalnum(ch) || ch == '_'; }

bool is_number(char *literal) {
  int size = strlen(literal);
  int i;
  for (i = 0; i < size; ++i) {
    if (!isdigit(literal[i])) {
      return false;
    }
  }
  return true;
}

uint32_t hash_string(char *str) {
  uint32_t hash_value;
  for (hash_value = 0; *str != '\0'; str++)
    hash_value = *str + hash_value * 31;
  return hash_value % HASH_TABLE_SIZE;
}

uint64_t hash_pointer(uint64_t pointer) {
  uint64_t hash_value;
  hash_value = ((pointer >> 16) ^ pointer) * 0x45d9f3b;
  hash_value = ((pointer >> 16) ^ pointer) * 0x45d9f3b;
  hash_value = (pointer >> 16) ^ pointer;
  return hash_value % HASH_TABLE_SIZE;
}

hash_table_t *create_hash_table() {
  hash_table_t *hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));
  hash_table->length = 0;
  hash_table->capacity = HASH_TABLE_SIZE;
  hash_item_t **associative_array =
      (hash_item_t **)calloc(HASH_TABLE_SIZE, sizeof(hash_item_t));
  for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
    associative_array[i] = NULL;
  }
  hash_table->items = associative_array;
  return hash_table;
}

hash_table_pointer_expression_t *create_pointer_expression_hash_table() {
  hash_table_pointer_expression_t *hash_table =
      (hash_table_pointer_expression_t *)malloc(
          sizeof(hash_table_pointer_expression_t));
  hash_table->length = 0;
  hash_table->capacity = HASH_TABLE_SIZE;
  hash_item_pointer_expression_t **associative_array =
      (hash_item_pointer_expression_t **)calloc(
          HASH_TABLE_SIZE, sizeof(hash_table_pointer_expression_t));
  for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
    associative_array[i] = NULL;
  }
  hash_table->items = associative_array;
  return hash_table;
}

hash_table_object_object_t *create_object_object_hash_table() {
  hash_table_object_object_t *hash_table =
      (hash_table_object_object_t *)malloc(sizeof(hash_table_object_object_t));
  hash_table->length = 0;
  hash_table->capacity = HASH_TABLE_SIZE;
  hash_pair_t **associative_array =
      (hash_pair_t **)calloc(HASH_TABLE_SIZE, sizeof(hash_pair_t));
  for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
    associative_array[i] = NULL;
  }
  hash_table->items = associative_array;
  return hash_table;
}

bool insert_hash_table(hash_table_t *hash_table, char *key, object_t *value) {
  uint32_t hash_value = hash_string(key);
  hash_item_t *hash_item = (hash_item_t *)malloc(sizeof(hash_item_t));
  hash_item->key = hash_value;
  hash_item->value = value;
  hash_table->items[hash_value] = hash_item;
  hash_table->length++;
  return true;
}

bool insert_pointer_expression_hash_table(
    hash_table_pointer_expression_t *hash_table, uint64_t key,
    expression_t *value) {
  uint64_t hash_value = hash_pointer(key);
  hash_item_pointer_expression_t *hash_item =
      (hash_item_pointer_expression_t *)malloc(
          sizeof(hash_item_pointer_expression_t));
  hash_item->key = key;
  hash_item->value = value;
  hash_table->items[hash_value] = hash_item;
  hash_table->length++;
  return true;
}

bool insert_object_object_hash_table(hash_table_object_object_t *hash_table,
                                     object_t *key, object_t *value) {
  hash_key_t *hash_key = hash_object(key);
  hash_pair_t *hash_pair = (hash_pair_t *)malloc(sizeof(hash_pair_t));
  hash_pair->key = key;
  hash_pair->value = value;
  hash_table->items[hash_key->value] = hash_pair;
  hash_table->length++;
  return true;
}

void free_hash_table(hash_table_t *hash_table) {
  for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
    if (hash_table->items[i] != NULL) {
      FREE(hash_table->items[i]->key);
      free_object(hash_table->items[i]->value);
    }
  }
  FREE(hash_table);
}

object_t *get_value_hash_table(hash_table_t *hash_table, char *key) {
  uint32_t hash_value = hash_string(key);
  return hash_table->items[hash_value] == NULL
             ? NULL
             : hash_table->items[hash_value]->value;
}

expression_t *
get_pointer_expression_hash_table(hash_table_pointer_expression_t *hash_table,
                                  uint64_t key) {
  uint64_t hash_value = hash_pointer(key);
  return hash_table->items[hash_value] == NULL
             ? NULL
             : hash_table->items[hash_value]->value;
}

hash_pair_t *get_hash_pair_hash_table(hash_table_object_object_t *hash_table,
                                      object_t *key) {
  hash_key_t *hash_key = hash_object(key);
  return hash_table->items[hash_key->value] == NULL
             ? NULL
             : hash_table->items[hash_key->value];
}
