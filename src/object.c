#include "object.h"
#include "nodes.h"
#include "parser.h"
#include "utils.h"
#include <assert.h>
#include <string.h>

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

char *get_concated_arguments_string(expression_t **parameters,
                                    size_t parameters_length) {
  char *concated_arg_string = (char *)malloc(STRING_MAX_SIZE);
  concated_arg_string[0] = '\0';
  for (int i = 0; i < parameters_length; ++i) {
    expression_t *iden = parameters[i];
    strcat(concated_arg_string, iden->node.string((void *)iden));
    if (i != parameters_length - 1) {
      strcat(concated_arg_string, ", ");
    }
  }
  return concated_arg_string;
}

char *get_fn_body_string(block_statement_t *block_stmt) {
  return block_stmt->statement.node.string((void *)block_stmt);
}

char *inspect_function(void *function_object_cast_to_void) {
  function_obj_t *func_obj = (function_obj_t *)function_object_cast_to_void;
  char *func_obj_string = (char *)malloc(STRING_MAX_SIZE);
  snprintf(func_obj_string, STRING_MAX_SIZE, "fn(%s){\n %s }",
           get_concated_arguments_string(func_obj->parameters,
                                         func_obj->parameters_length),
           get_fn_body_string(func_obj->body));
  return func_obj_string;
}

object_type type_function() { return FUNCTION_OBJ; }

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
