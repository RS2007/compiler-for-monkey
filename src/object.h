#pragma once

typedef enum object_type {
  INTEGER,
  BOOLEAN,
  NULL_OBJ,
} object_type;

typedef object_type (*Type)();
typedef char *(*Inspect)(void *);

static char *object_type_strings[] = {
    "INTEGER",
    "BOOLEAN",
    "NULL",
};

typedef struct object_t {
  Type type;
  Inspect inspect;
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

object_type type_int();
object_type type_boolean();
object_type type_null();
char *inspect_int(void *);
char *inspect_boolean(void *);
char *inspect_null(void *);
