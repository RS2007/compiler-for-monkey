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

typedef enum hash_key_type {
  HASH_KEY_POINTER_TYPE,
  HASH_KEY_HASH_KEY_TYPE,
  HASH_KEY_STRING_TYPE,
} hash_key_type;

typedef enum hash_value_type {
  HASH_VALUE_TYPE_EXPRESSION,
  HASH_VALUE_TYPE_OBJECT,
  HASH_VALUE_TYPE_HASH_PAIR,
} hash_value_type;

typedef enum linked_list_type {
  LL_KEY_VALUE_TYPE,
} linked_list_type;

typedef struct generic_key_value_t {
  hash_key_type key_type;
  hash_value_type value_type;
  void *key;
  void *value;
} generic_key_value_t;

typedef struct generic_linked_list_node_t {
  linked_list_type type;
  void *data;
  struct generic_linked_list_node_t *next;
} generic_linked_list_node_t;

typedef struct generic_linked_list_t {
  linked_list_type type;
  generic_linked_list_node_t *head;
  generic_linked_list_node_t *tail;
} generic_linked_list_t;

typedef struct generic_hash_table_t {
  hash_key_type key_type;
  hash_value_type value_type;
  generic_linked_list_t **items;
  size_t occupied_size;
} generic_hash_table_t;

uint64_t hash_pointer(uint64_t);

generic_hash_table_t *create_hash_table(hash_key_type key_type,
                                        hash_value_type value_type);
bool insert_hash_table(generic_hash_table_t *hash_table, void *key,
                       void *value);

void *get_value_hash_table(generic_hash_table_t *hash_table, void *key);

void free_hash_table(generic_hash_table_t *);
