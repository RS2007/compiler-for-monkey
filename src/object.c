#include "object.h"
#include "parser.h"
#include "utils.h"
#include <assert.h>

char *inspect_int(void *int_object_cast_to_void) {
  integer_obj_t *int_object = (integer_obj_t *)int_object_cast_to_void;
  char *int_object_string = (char *)malloc(STRING_MAX_SIZE);
  snprintf(int_object_string, STRING_MAX_SIZE, "%lld", int_object->value);
  return int_object_string;
}

object_type type_int() { return INTEGER; }

char *inspect_boolean(void *boolean_object_cast_to_void) {
  boolean_obj_t *boolean_obj = (boolean_obj_t *)boolean_object_cast_to_void;
  char *boolean_object_string = (char *)malloc(6);
  bool is_true = boolean_obj->value == 1;
  bool is_false = boolean_obj->value == 0;
  if (is_true && is_false) {
    assert(false);
    exit(-1);
  }
  snprintf(boolean_object_string, 6, "%s", is_true ? "true" : "false");
  return boolean_object_string;
}

object_type type_boolean() { return BOOLEAN; }

char *inspect_null(void *null_object_cast_to_void) {
  char *null_string = (char *)malloc(5);
  snprintf(null_string, 5, "%s", "NULL");
  return null_string;
}

object_type type_null() { return NULL_OBJ; }

char *inspect_return(void *return_object_cast_to_void) {
  return_obj_t *return_obj = (return_obj_t *)return_object_cast_to_void;
  return return_obj->value->inspect((void *)return_obj->value);
}

object_type type_return() { return RETURN_VALUE_OBJ; }

char *inspect_error(void *error_obj_cast_to_void) {
  error_obj_t *error_obj = (error_obj_t *)error_obj_cast_to_void;
  snprintf(error_obj->message, STRING_MAX_SIZE, "ERROR: %s",
           error_obj->message);
  return error_obj->message;
}

object_type type_error() { return ERROR_OBJ; }
