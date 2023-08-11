#pragma once
typedef struct object_t object_t;
typedef struct environment_t environment_t;

#include "nodes.h"
#include <stdbool.h>

typedef enum object_type {
  INTEGER,
  BOOLEAN,
  FUNCTION_OBJ,
  NULL_OBJ,
  RETURN_VALUE_OBJ,
  ERROR_OBJ,
  STRING_OBJ,
  BUILTIN_OBJ
} object_type;

typedef object_type (*Type)();
typedef char *(*Inspect)(void *);

static char *object_type_strings[] = {"INTEGER", "BOOLEAN", "FUNCTION",
                                      "NULL",    "RETURN",  "ERROR",
                                      "STRING",  "BUILTIN"};

typedef struct object_t {
  Type type;
  Inspect inspect;
  size_t refcount;
} object_t;
typedef struct integer_obj_t {
  object_t object;
  long long value;
} integer_obj_t;

typedef struct boolean_obj_t {
  object_t object;
  bool value;
} boolean_obj_t;

typedef struct null_obj_t {
  object_t object;
} null_obj_t;

typedef struct return_obj_t {
  object_t object;
  object_t *value;
} return_obj_t;

typedef struct error_obj_t {
  object_t object;
  char *message;
} error_obj_t;

typedef struct function_obj_t {
  object_t object;
  expression_t **parameters;
  size_t parameters_length;
  size_t parameters_capacity;
  block_statement_t *body;
  environment_t *env;
} function_obj_t;

typedef struct string_obj_t {
  object_t object;
  char *value;
} string_obj_t;

typedef struct builtin_obj_t {
  object_t object;
  char *name;
} builtin_obj_t;

object_type type_int();
object_type type_boolean();
object_type type_function();
object_type type_null();
object_type type_return();
object_type type_error();
object_type type_string();
object_type type_builtin();
char *inspect_int(void *);
char *inspect_boolean(void *);
char *inspect_function(void *);
char *inspect_null(void *);
char *inspect_return(void *);
char *inspect_error(void *);
char *inspect_string(void *);
char *inspect_builtin(void *);
void free_object(object_t *);
void free_statement(statement_t *);
void free_expression(expression_t *);
