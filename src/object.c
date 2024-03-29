#include "object.h"
#include "environment.h"
#include "nodes.h"
#include "parser.h"
#include "utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void free_expression (expression_t *);
void free_block_statement (block_statement_t *);

char *
inspect_int (void *int_object_cast_to_void)
{
    integer_obj_t *int_object = (integer_obj_t *)int_object_cast_to_void;
    char *int_object_string = (char *)malloc (STRING_MAX_SIZE);
    snprintf (int_object_string, STRING_MAX_SIZE, "%lld", int_object->value);
    return int_object_string;
}

object_type
type_int ()
{
    return INTEGER;
}

char *
inspect_boolean (void *boolean_object_cast_to_void)
{
    boolean_obj_t *boolean_obj = (boolean_obj_t *)boolean_object_cast_to_void;
    char *boolean_object_string = (char *)malloc (6);
    bool is_true = boolean_obj->value == 1;
    bool is_false = boolean_obj->value == 0;
    if (is_true && is_false)
        {
            assert (false);
            exit (-1);
        }
    snprintf (boolean_object_string, 6, "%s", is_true ? "true" : "false");
    return boolean_object_string;
}

object_type
type_boolean ()
{
    return BOOLEAN;
}

char *
inspect_builtin (void *builtin_object_cast_to_void)
{
    char *buffer = malloc (STRING_MAX_SIZE);
    buffer = "Builtin function";
    return buffer;
}

object_type
type_builtin ()
{
    return BUILTIN_OBJ;
}

char *
get_concated_arguments_string (expression_t **parameters,
                               size_t parameters_length)
{
    char *concated_arg_string = (char *)malloc (STRING_MAX_SIZE);
    concated_arg_string[0] = '\0';
    for (int i = 0; i < parameters_length; ++i)
        {
            expression_t *iden = parameters[i];
            strcat (concated_arg_string, iden->node.string ((void *)iden));
            if (i != parameters_length - 1)
                {
                    strcat (concated_arg_string, ", ");
                }
        }
    return concated_arg_string;
}

char *
get_fn_body_string (block_statement_t *block_stmt)
{
    return block_stmt->statement.node.string ((void *)block_stmt);
}

char *
inspect_function (void *function_object_cast_to_void)
{
    function_obj_t *func_obj = (function_obj_t *)function_object_cast_to_void;
    char *func_obj_string = (char *)malloc (STRING_MAX_SIZE);
    snprintf (func_obj_string, STRING_MAX_SIZE, "fn(%s){\n %s \n}",
              get_concated_arguments_string (func_obj->parameters,
                                             func_obj->parameters_length),
              get_fn_body_string (func_obj->body));
    return func_obj_string;
}

object_type
type_function ()
{
    return FUNCTION_OBJ;
}

char *
inspect_null (void *null_object_cast_to_void)
{
    char *null_string = (char *)malloc (5);
    snprintf (null_string, 5, "%s", "NULL");
    return null_string;
}

object_type
type_null ()
{
    return NULL_OBJ;
}

char *
inspect_return (void *return_object_cast_to_void)
{
    return_obj_t *return_obj = (return_obj_t *)return_object_cast_to_void;
    return return_obj->value->inspect ((void *)return_obj->value);
}

object_type
type_return ()
{
    return RETURN_VALUE_OBJ;
}

char *
inspect_error (void *error_obj_cast_to_void)
{
    error_obj_t *error_obj = (error_obj_t *)error_obj_cast_to_void;
    snprintf (error_obj->message, STRING_MAX_SIZE, "ERROR: %s",
              error_obj->message);
    return error_obj->message;
}

object_type
type_error ()
{
    return ERROR_OBJ;
}

char *
inspect_string (void *string_obj_cast_to_void)
{
    string_obj_t *string_obj = (string_obj_t *)string_obj_cast_to_void;
    return string_obj->value;
}

object_type
type_string ()
{
    return STRING_OBJ;
}

char *
inspect_array (void *array_obj_cast_to_void)
{
    array_obj_t *array_obj = (array_obj_t *)array_obj_cast_to_void;
    char *buffer
        = (char *)malloc (array_obj->elements_length * STRING_MAX_SIZE);
    buffer[0] = '[';
    buffer[1] = '\0';
    for (int i = 0; i < array_obj->elements_length; ++i)
        {
            if (i != array_obj->elements_length - 1)
                {
                    strcat (buffer, array_obj->elements[i]->inspect (
                                        (void *)array_obj->elements[i]));
                    strcat (buffer, ", ");
                }
            else
                {
                    strcat (buffer, array_obj->elements[i]->inspect (
                                        (void *)array_obj->elements[i]));
                    strcat (buffer, "]");
                }
        }
    return buffer;
}

object_type
type_array ()
{
    return ARRAY_OBJ;
}

char *
inspect_hash (void *hash_cast_to_void)
{
    hash_obj_t *hash_obj = (hash_obj_t *)hash_cast_to_void;
    char *buffer = (char *)malloc (STRING_MAX_SIZE);
    buffer[0] = '{';
    buffer[1] = '\0';
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
        {
            generic_linked_list_t *hash_pair = hash_obj->pairs->items[i];
            if (hash_pair == NULL || hash_pair->head == NULL)
                {
                    continue;
                }
            generic_linked_list_node_t *runner = hash_pair->head;
            while (runner != NULL)
                {
                    generic_key_value_t *key_value
                        = (generic_key_value_t *)runner->data;
                    hash_pair_t *value_obj = (hash_pair_t *)key_value->value;
                    char *key_str
                        = value_obj->key->inspect ((void *)value_obj->key);
                    char *value_str
                        = value_obj->value->inspect ((void *)value_obj->value);
                    strcat (buffer, key_str);
                    strcat (buffer, " : ");
                    strcat (buffer, value_str);
                    strcat (buffer, ",");
                    runner = runner->next;
                }
        }
    buffer[strlen (buffer) - 1] = '}';
    return buffer;
}

object_type
type_hash ()
{
    return HASH_OBJ;
};

void
free_identifier (identifier_t *identifier)
{
    FREE (identifier->value);
    FREE (identifier->token);
    FREE (identifier);
}

void
free_statement (statement_t *statement)
{
    switch (statement->type)
        {
        case LET_STATEMENT:
            {
                let_statement_t *let_stmt = (let_statement_t *)statement;
                free_expression (let_stmt->value);
                free_identifier (let_stmt->name);
                FREE (let_stmt->token);
                FREE (let_stmt);
                break;
            }
        case RETURN_STATEMENT:
            {
                ret_statement_t *ret_stmt = (ret_statement_t *)statement;
                free_expression (ret_stmt->return_value);
                FREE (ret_stmt->token);
                FREE (ret_stmt);
                break;
            }
        case EXPRESSION_STATEMENT:
            {
                expression_statement_t *expr_statement
                    = (expression_statement_t *)statement;
                free_expression (expr_statement->expression);
                // TODO: check why this segfaults
                // FREE(expr_statement->token);
                FREE (expr_statement);
                break;
            }
        case BLOCK_STATEMENT:
            {
                block_statement_t *blk_statement
                    = (block_statement_t *)statement;
                free_block_statement (blk_statement);
                break;
            }
        }
}

void
free_block_statement (block_statement_t *statement)
{
    for (int i = 0; i < statement->statements_length; ++i)
        {
            free_statement (statement->statements[i]);
        }
    FREE (statement->token);
    FREE (statement);
}

void
free_expression (expression_t *expression)
{
    if (expression == NULL)
        return;
    switch (expression->type)
        {
        case INTEGER_LITERAL:
            {
                integer_t *integer = (integer_t *)expression;
                FREE (integer->token);
                if (integer == NULL)
                    assert (0 && "This is wrong");
                FREE (integer);
                break;
            }
        case IDENTIFIER:
            {
                identifier_t *identifier = (identifier_t *)expression;
                FREE (identifier->token);
                FREE (identifier->value);
                FREE (identifier);
                break;
            }
        case BOOLEAN_LITERAL:
            {
                boolean_expression_t *boolean
                    = (boolean_expression_t *)expression;
                FREE (boolean->token);
                FREE (boolean);
                break;
            }
        case PREFIX_EXPRESSION:
            {
                prefix_expression_t *prefix_expr
                    = (prefix_expression_t *)expression;
                FREE (prefix_expr->token);
                free_expression (prefix_expr->right);
                FREE (prefix_expr->op);
                FREE (prefix_expr);
                break;
            }
        case INFIX_EXPRESSION:
            {
                infix_expression_t *infix_expr
                    = (infix_expression_t *)expression;
                FREE (infix_expr->token);
                free_expression (infix_expr->left);
                free_expression (infix_expr->right);
                FREE (infix_expr->op);
                FREE (infix_expr);
                break;
            }
        case IF_EXPRESSION:
            {
                if_expression_t *if_expr = (if_expression_t *)expression;
                FREE (if_expr->token);
                free_expression (if_expr->condition);
                free_block_statement (if_expr->consequence);
                free_block_statement (if_expr->alternative);
                break;
            }
        case FUNCTION_EXPRESSION:
            {
                function_literal_t *func_literal
                    = (function_literal_t *)expression;
                FREE (func_literal->token);
                free_expression (func_literal->function);
                break;
            }
        case STRING_LITERAL:
            {
                string_literal_t *string_literal
                    = (string_literal_t *)expression;
                FREE (string_literal->token);
                FREE (string_literal->value);
                FREE (string_literal);
                break;
            }
        case CALL_EXPRESSION:
            {
                call_expression_t *call_expression
                    = (call_expression_t *)expression;
                for (int i = 0; i < call_expression->arguments_length; ++i)
                    {
                        FREE (call_expression->arguments[i]);
                    }
                free_expression (call_expression->function);
                FREE (call_expression->token);
                FREE (call_expression);
            }
        }
}

void
free_function_parameters (expression_t **parameters,
                          unsigned int parameters_length)
{
    for (int i = 0; i < parameters_length; ++i)
        {
            free_expression (parameters[i]);
        }
}

void
free_function_body (block_statement_t *body)
{
    free_block_statement (body);
}

void
free_environment (environment_t *env)
{
    if (env->outer != NULL)
        {
            free_environment (env->outer);
        }
    FREE (env->outer);
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
        {
            if (env->store->items[i]->head != NULL)
                {
                    generic_linked_list_node_t *runner
                        = env->store->items[i]->head;
                    while (runner != NULL)
                        {
                            FREE (runner->data);
                            generic_linked_list_node_t *old_runner = runner;
                            runner = runner->next;
                            FREE (old_runner);
                        }
                }
        }
    free_hash_table (env->store);
}

void
free_function_obj (function_obj_t *function_object)
{
    free_function_parameters (function_object->parameters,
                              function_object->parameters_length);
    free_function_body (function_object->body);
    free_environment (function_object->env);
    FREE (function_object);
}

uint64_t
hash_integer (long long value)
{
    return hash_pointer (value);
}

uint64_t
hash_string_64 (char *str)
{
    uint64_t hash_value;
    for (hash_value = 0; *str != '\0'; str++)
        hash_value = *str + hash_value * 63;
    return hash_value % HASH_TABLE_SIZE;
}
uint64_t
hash_boolean (bool boolean)
{
    return (uint64_t)boolean;
}

hash_key_t *
hash_object (object_t *key)
{
    switch (key->type ())
        {
        case INTEGER:
            {
                integer_obj_t *integer = (integer_obj_t *)key;
                hash_key_t *hash_key = malloc (sizeof (hash_key_t));
                hash_key->value
                    = hash_integer (integer->value) % HASH_TABLE_SIZE;
                hash_key->type = INTEGER;
                return hash_key;
            }
        case BOOLEAN:
            {
                boolean_obj_t *boolean = (boolean_obj_t *)key;
                hash_key_t *hash_key = malloc (sizeof (hash_key_t));
                hash_key->value
                    = hash_boolean (boolean->value) % HASH_TABLE_SIZE;
                hash_key->type = BOOLEAN;
                return hash_key;
            }
        case STRING_OBJ:
            {
                string_obj_t *string = (string_obj_t *)key;
                hash_key_t *hash_key = malloc (sizeof (hash_key_t));
                hash_key->value
                    = hash_string_64 (string->value) % HASH_TABLE_SIZE;
                hash_key->type = STRING_OBJ;
                return hash_key;
            }
        default:
            assert (0 && "Should not hit here");
        }
    return NULL;
}

void
free_object (object_t *object)
{
    if (object->refcount > 0)
        {
            return;
        }
    switch (object->type ())
        {
        case INTEGER:
            {
                integer_obj_t *integer_object = (integer_obj_t *)object;
                FREE (integer_object);
                break;
            }
        case BOOLEAN:
            {
                boolean_obj_t *boolean_object = (boolean_obj_t *)object;
                FREE (boolean_object);
                break;
            }
        case FUNCTION_OBJ:
            {
                function_obj_t *function_object = (function_obj_t *)object;
                free_function_obj (function_object);
                break;
            }
        case NULL_OBJ:
            {
                null_obj_t *null_object = (null_obj_t *)object;
                FREE (null_object);
                break;
            }
        case RETURN_VALUE_OBJ:
            {
                return_obj_t *ret_object = (return_obj_t *)object;
                free_object (ret_object->value);
                FREE (ret_object);
                break;
            }
        case ERROR_OBJ:
            {
                error_obj_t *error_obj = (error_obj_t *)object;
                FREE (error_obj->message);
                FREE (error_obj);
                break;
            }
        case STRING_OBJ:
            {
                string_obj_t *string_obj = (string_obj_t *)object;
                FREE (string_obj->value);
                FREE (string_obj);
                break;
            }
        case BUILTIN_OBJ:
            {
                break;
            }
        }
}
