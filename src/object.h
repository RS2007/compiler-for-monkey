#pragma once
typedef struct object_t object_t;
typedef struct statement_t statement_t;
typedef struct block_statement_t block_statement_t;
typedef struct expression_t expression_t;
typedef struct environment_t environment_t;
typedef struct generic_hash_table_t generic_hash_table_t;

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum object_type
{
    INTEGER,
    BOOLEAN,
    FUNCTION_OBJ,
    NULL_OBJ,
    RETURN_VALUE_OBJ,
    ERROR_OBJ,
    STRING_OBJ,
    BUILTIN_OBJ,
    ARRAY_OBJ,
    HASH_OBJ,
} object_type;

typedef object_type (*Type) ();
typedef char *(*Inspect) (void *);

static char *object_type_strings[]
    = { "INTEGER", "BOOLEAN", "FUNCTION", "NULL",  "RETURN",
        "ERROR",   "STRING",  "BUILTIN",  "ARRAY", "HASH" };

typedef struct object_t
{
    Type type;
    Inspect inspect;
    size_t refcount;
} object_t;
typedef struct integer_obj_t
{
    object_t object;
    long long value;
} integer_obj_t;

typedef struct boolean_obj_t
{
    object_t object;
    bool value;
} boolean_obj_t;

typedef struct null_obj_t
{
    object_t object;
} null_obj_t;

typedef struct return_obj_t
{
    object_t object;
    object_t *value;
} return_obj_t;

typedef struct error_obj_t
{
    object_t object;
    char *message;
} error_obj_t;

typedef struct function_obj_t
{
    object_t object;
    expression_t **parameters;
    size_t parameters_length;
    size_t parameters_capacity;
    block_statement_t *body;
    environment_t *env;
} function_obj_t;

typedef struct string_obj_t
{
    object_t object;
    char *value;
} string_obj_t;

typedef struct builtin_obj_t
{
    object_t object;
    char *name;
} builtin_obj_t;

typedef struct array_obj_t
{
    object_t object;
    object_t **elements;
    size_t elements_length;
    size_t elements_capacity;
} array_obj_t;

typedef struct hash_pair_t
{
    object_t *key;
    object_t *value;
} hash_pair_t;

typedef struct hash_key_t
{
    object_type type;
    uint64_t value;
} hash_key_t;

typedef struct hash_obj_t
{
    object_t object;
    generic_hash_table_t *pairs;
} hash_obj_t;

hash_key_t *hash_object (object_t *);

object_type type_int ();
object_type type_boolean ();
object_type type_function ();
object_type type_null ();
object_type type_return ();
object_type type_error ();
object_type type_string ();
object_type type_builtin ();
object_type type_array ();
object_type type_hash ();
char *inspect_int (void *);
char *inspect_boolean (void *);
char *inspect_function (void *);
char *inspect_null (void *);
char *inspect_return (void *);
char *inspect_error (void *);
char *inspect_string (void *);
char *inspect_builtin (void *);
char *inspect_array (void *);
char *inspect_hash (void *);
void free_object (object_t *);
void free_statement (statement_t *);
void free_expression (expression_t *);
