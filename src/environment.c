#include "environment.h"

environment_t *new_environment() {
  environment_t *env = (environment_t *)malloc(sizeof(environment_t));
  env->store = create_hash_table();
  return env;
}

object_t *get_environment(environment_t *environment, char *key) {
  return get_value_hash_table(environment->store, key);
}

object_t *set_environment(environment_t *environment, char *key,
                          object_t *value) {
  insert_hash_table(environment->store, key, value);
  return value;
}
