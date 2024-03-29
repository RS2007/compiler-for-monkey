#pragma once

#include "token.h"
#include "utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum node_type
{
    PROGRAM,
    STATEMENT,
    EXPRESSION,
} node_type;

typedef enum statement_type
{
    LET_STATEMENT,
    RETURN_STATEMENT,
    EXPRESSION_STATEMENT,
    BLOCK_STATEMENT,
} statement_type;

typedef enum expression_type
{
    INTEGER_LITERAL,
    IDENTIFIER,
    BOOLEAN_LITERAL,
    PREFIX_EXPRESSION,
    INFIX_EXPRESSION,
    IF_EXPRESSION,
    FUNCTION_EXPRESSION,
    CALL_EXPRESSION,
    STRING_LITERAL,
    ARRAY_LITERAL,
    INDEX_EXPRESSION,
    HASH_LITERAL,
} expression_type;

typedef char *(*Token_literal) (void *);
typedef char *(*Value) (void *);
typedef void (*Statement_node) (void *);
typedef void (*Expression_node) (void *);
typedef char *(*String) (void *);

typedef struct node_t
{
    node_type type;
    Token_literal token_literal;
    String string;
} node_t;

typedef struct statement_t
{
    node_t node;
    statement_type type;
} statement_t;

typedef struct expression_t
{
    node_t node;
    expression_type type;
} expression_t;

typedef struct program_t
{
    node_t node; // struct composition
    statement_t **statements;
    size_t statements_length;
    size_t statements_capacity;
} program_t;

typedef struct identifier_t
{
    expression_t expression; // struct composition
    token_t *token;
    char *value;
} identifier_t;

typedef struct integer_t
{
    expression_t expression;
    token_t *token;
    long long value; // CHECK: not sure about the type here
} integer_t;

typedef struct prefix_expression_t
{
    expression_t expression;
    token_t *token;
    expression_t *right;
    char *op;
} prefix_expression_t;

typedef struct infix_expression_t
{
    expression_t expression;
    token_t *token;
    expression_t *left;
    expression_t *right;
    char *op;
} infix_expression_t;

typedef struct string_literal_t
{
    expression_t expression;
    token_t *token;
    char *value;
} string_literal_t;

typedef struct let_statement_t
{
    statement_t statement;
    token_t *token;
    identifier_t *name;
    expression_t *value;
} let_statement_t;

typedef struct ret_statement_t
{
    statement_t statement;
    token_t *token;
    expression_t *return_value;
} ret_statement_t;

typedef struct expression_statement_t
{
    statement_t statement;
    token_t *token;
    expression_t *expression;
} expression_statement_t;

typedef struct block_statement_t
{
    statement_t statement;
    token_t *token;
    statement_t **statements;
    size_t statements_length;
    size_t statements_capacity;
} block_statement_t;

typedef struct boolean_expression_t
{
    expression_t expression;
    token_t *token;
    bool value;
} boolean_expression_t;

typedef struct if_expression_t
{
    expression_t expression;
    token_t *token;
    expression_t *condition;
    block_statement_t *consequence;
    block_statement_t *alternative;
} if_expression_t;

typedef struct function_literal_t
{
    expression_t expression;
    token_t *token;
    expression_t *function;
    expression_t **arguments;
    size_t arguments_length;
    size_t arguments_capacity;
    block_statement_t *body;
} function_literal_t;

typedef struct call_expression_t
{
    expression_t expression;
    token_t *token;
    expression_t *function;
    expression_t **arguments;
    size_t arguments_length;
    size_t arguments_capacity;
} call_expression_t;

typedef struct array_literal_t
{
    expression_t expression;
    token_t *token;
    expression_t **elements;
    size_t elements_length;
    size_t elements_capacity;
} array_literal_t;

typedef struct index_expression_t
{
    expression_t expression;
    token_t *token;
    expression_t *left;
    expression_t *index;
} index_expression_t;

typedef struct hash_literal_t
{
    expression_t expression;
    token_t *token;
    generic_hash_table_t *pairs;
} hash_literal_t;

typedef expression_t *(*prefix_parse_function) ();
typedef expression_t *(*infix_parse_function) ();

expression_t *parse_identifier ();
expression_t *parse_integer_literal ();
expression_t *parse_prefix_expression ();
expression_t *parse_boolean_expression ();
expression_t *parse_grouped_expression ();
expression_t *parse_if_expression ();
expression_t *parse_function_expression ();
expression_t *parse_call_expression ();
expression_t *parse_string_literal ();
expression_t *parse_array_literal ();
expression_t *parse_hash_literal ();

expression_t *parse_infix_function ();
expression_t *parse_index_expression ();

static prefix_parse_function prefix_parse_functions[]
    = { NULL, NULL, parse_identifier, parse_integer_literal, NULL, NULL,
        parse_prefix_expression, parse_prefix_expression, NULL, NULL,
        parse_prefix_expression, parse_prefix_expression, NULL, NULL,
        parse_grouped_expression, NULL, parse_hash_literal, NULL,
        parse_function_expression, NULL, parse_boolean_expression,
        parse_boolean_expression, parse_if_expression, NULL, NULL, NULL,

        // Equality
        NULL, parse_string_literal, parse_array_literal, NULL, NULL };

static infix_parse_function infix_parse_functions[] = {
    NULL,                 // ILLEGAL
    NULL,                 // END_OF_FILE
    NULL,                 // IDENT
    NULL,                 // INT
    NULL,                 // ASSIGN
    parse_infix_function, // SUM:PLUS
    NULL,                 // BANG
    parse_infix_function, // SUM,:MINUS
    parse_infix_function, // PRODUCT:SLASH,
    parse_infix_function, // PRODUCT:ASTERIK,
    parse_infix_function, // LESSGREATER:LT,
    parse_infix_function, // LESSGREATER:GT,

    // Delimiters
    NULL, // COMMA
    NULL, // SEMICOLON

    parse_call_expression, // LPAREN
    NULL,                  // RPAREN
    NULL,                  // LBRACE
    NULL,                  // RBRACE

    // Keywords
    NULL, // FUNCTION
    NULL, // LET
    NULL, // TRUE
    NULL, // FALSE
    NULL, // IF
    NULL, // ELSE
    NULL, // RETURN

    parse_infix_function,   // EQUALS,
    parse_infix_function,   // NOTEQ
    NULL,                   // STRING
    parse_index_expression, // LBRACKET
    NULL,                   // RBRACKET
    NULL,                   // COLON
    parse_infix_function,   // LT_EQ
    parse_infix_function    // RT_EQ
};
