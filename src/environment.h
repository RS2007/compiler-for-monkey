#pragma once

#include "object.h"
#include "utils.h"

typedef struct environment_t {
  generic_hash_table_t *store;
  environment_t *outer;
} environment_t;

environment_t *new_environment();
object_t *get_environment(environment_t *environment, char *key);
object_t *set_environment(environment_t *environment, char *key,
                          object_t *value);
