#include "environment.h"

environment_t *new_environment() {
  environment_t *env = (environment_t *)malloc(sizeof(environment_t));
  env->store = create_hash_table(HASH_KEY_STRING_TYPE, HASH_VALUE_TYPE_OBJECT);
  env->outer = NULL;
  return env;
}

object_t *get_environment(environment_t *environment, char *key) {
  generic_key_value_t *key_value =
      (generic_key_value_t *)get_value_hash_table(environment->store, key);
  if (key_value == NULL) {
    return NULL;
  }
  object_t *value = (object_t *)key_value->value;
  if (value == NULL && environment->outer != NULL) {

    return get_environment(environment->outer, key);
  };
  return value;
}

object_t *set_environment(environment_t *environment, char *key,
                          object_t *value) {
  char *duplicated_key = strdup(
      key); // Duplication needed since expression gets freed on execution
  insert_hash_table(environment->store, duplicated_key, value);
  value->refcount++;
  return value;
}
