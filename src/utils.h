#pragma once
#include "object.h"
#include <stdbool.h>
#include <stdlib.h>
bool is_character(char);
bool is_number(char *);

#define unimplemented() printf("\nUnimplemented\n")
#define DEFAULT_DYNAMIC_ARR_SIZE 10
#define STRING_MAX_SIZE 1024
#define MAX_ERROR_SIZE 1024
#define HASH_TABLE_SIZE 5000

#define FREE(ptr)                                                              \
  do {                                                                         \
    if (ptr != NULL) {                                                         \
      free(ptr);                                                               \
    }                                                                          \
    (ptr) = NULL;                                                              \
  } while (0)

typedef struct hash_item_t {
  char *key;
  object_t *value;
} hash_item_t;

typedef struct hash_table_t {
  hash_item_t **items;
  size_t length;
  size_t capacity;
} hash_table_t;

hash_table_t *create_hash_table();
bool insert_hash_table(hash_table_t *hash_table, char *key, object_t *value);
object_t *get_value_hash_table(hash_table_t *hash_table, char *key);
void free_hash_table(hash_table_t *);
