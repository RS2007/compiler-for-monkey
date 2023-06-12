#include "environment.h"

environment_t *new_environment() {
  environment_t *env = (environment_t *)malloc(sizeof(environment_t));
  env->store = create_hash_table();
  env->outer = NULL;
  return env;
}

object_t *get_environment(environment_t *environment, char *key) {
  object_t *value;
  value = get_value_hash_table(environment->store,key);
  if (value == NULL &&
      environment->outer != NULL) {

    return get_environment(environment->outer, key);
  };
  return value;
}

object_t *set_environment(environment_t *environment, char *key,
                          object_t *value) {
  insert_hash_table(environment->store, key, value);
  return value;
}
