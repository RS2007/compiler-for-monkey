#pragma once
#include "object.h"
#include <stdbool.h>
#include <stdint.h>
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

typedef struct hash_item_pointer_expression_t {
  uint64_t key;
  expression_t *value;
} hash_item_pointer_expression_t;

typedef struct hash_table_object_object_t {
  hash_pair_t **items;
  size_t length;
  size_t capacity;
} hash_table_object_object_t;

typedef struct hash_table_pointer_expression_t {
  hash_item_pointer_expression_t **items;
  size_t length;
  size_t capacity;
} hash_table_pointer_expression_t;

typedef struct hash_table_t {
  hash_item_t **items;
  size_t length;
  size_t capacity;
} hash_table_t;

uint64_t hash_pointer(uint64_t);

hash_table_t *create_hash_table();
hash_table_pointer_expression_t *create_pointer_expression_hash_table();
hash_table_object_object_t *create_object_object_hash_table();

bool insert_hash_table(hash_table_t *hash_table, char *key, object_t *value);
bool insert_pointer_expression_hash_table(
    hash_table_pointer_expression_t *hash_table, uint64_t key,
    expression_t *value);
bool insert_object_object_hash_table(hash_table_object_object_t *, object_t *,
                                     object_t *);

object_t *get_value_hash_table(hash_table_t *hash_table, char *key);
expression_t *
get_pointer_expression_hash_table(hash_table_pointer_expression_t *hash_table,
                                  uint64_t key);
hash_pair_t *get_hash_pair_hash_table(hash_table_object_object_t *hash_table,
                                      object_t *);

void free_hash_table(hash_table_t *);
void free_pointer_expression_hash_table(hash_table_pointer_expression_t *);
