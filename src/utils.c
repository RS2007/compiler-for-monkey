#include "utils.h"
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

hash_table_t *create_hash_table() {
  hash_table_t *hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));
  hash_table->length = 0;
  hash_table->capacity = HASH_TABLE_SIZE;
  hash_item_t **associative_array =
      (hash_item_t **)calloc(HASH_TABLE_SIZE, sizeof(hash_item_t));
  hash_table->items = associative_array;
  return hash_table;
}

bool insert_hash_table(hash_table_t *hash_table, char *key, object_t *value) {
  uint32_t hash_value = hash_string(key);
  hash_item_t* hash_item = (hash_item_t*)malloc(sizeof(hash_item_t));
  hash_item->key = hash_value;
  hash_item->value = value;
  hash_table->items[hash_value] = hash_item;
  hash_table->length++;
  return true;
}

object_t *get_value_hash_table(hash_table_t *hash_table, char *key) {
  uint32_t hash_value = hash_string(key);
  return hash_table->items[hash_value] == NULL ? hash_table->items[hash_value]: hash_table->items[hash_value]->value;
}
