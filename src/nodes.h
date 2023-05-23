#pragma once

#include "token.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum node_type {
  PROGRAM,
  STATEMENT,
  EXPRESSION,
} node_type;

typedef enum statement_type {
  LET_STATEMENT,
} statement_type;

typedef enum expression_type {
  UNIMPLEMENTED,
} expression_type;

typedef char *(*Token_literal)(void *);
typedef char *(*Value)(void *);
typedef void (*Statement_node)(void *);
typedef void (*Expression_node)(void *);
typedef char *(*String)(void *);

typedef struct expression_t {
  node_type type;
  token_type token;
  Token_literal token_literal;
  Value value;
  Expression_node expression_node;
  String string;
  int int_value;
  struct expression_t *right;
  char *op;
  struct expression_t *left;
  bool boolean_value;
} expression_t;

typedef struct statement_t {
  node_type type;
  Token_literal token_literal;
  Value value;
  char *iden_value;
  Statement_node statement_node;
  token_type token;
  char *iden_name;
  expression_t *expression;
  String string;
  struct statement_t **statements;
  int statements_length;
  int statements_capacity;
} statement_t;

typedef struct program_t {
  node_type type;
  Token_literal token_literal;
  Value value;
  // a dynamic array for statements
  statement_t **statements;
  int statements_size;
  int statements_capacity;
  String string;
} program_t;

typedef struct let_statement_t {
  node_type type;
  Token_literal token_literal;
  Value value;
  char *iden_value;
  Statement_node statement_node;
  token_type token;
  char *iden_name;
  expression_t *expression;
  String string;
  // TODO: this field is not correct, fix this asap expression_type value;
  statement_t **statements;
  int statements_length;
  int statements_capacity;
} let_statement_t;

typedef struct ret_statement_t {
  node_type type;
  Token_literal token_literal;
  Value value;
  char *iden_value;
  Statement_node statement_node;
  token_type token;
  char *iden_name;
  expression_t *expression;
  String string;
  statement_t **statements;
  int statements_length;
  int statements_capacity;
} ret_statement_t;

typedef struct block_statement_t {
  node_type type;
  Token_literal token_literal;
  Value value;
  char *iden_value;
  Statement_node statement_node;
  token_type token;
  char *iden_name;
  expression_t *expression;
  String string;
  // TODO: this field is not correct, fix this asap expression_type value;
  statement_t **statements;
  int statements_length;
  int statements_capacity;
} block_statement_t;

typedef struct expression_statement_t {
  node_type type;
  Token_literal token_literal;
  Value value;
  char *iden_value;
  Statement_node statement_node;
  token_type token;
  char *iden_name;
  expression_t *expression;
  String string;
  statement_t **statements;
  int statements_length;
  int statements_capacity;
} expression_statement_t;

typedef struct integer_literal_expression_t {
  node_type type;
  token_type token;
  Token_literal token_literal;
  Value value;
  Expression_node expression_node;
  String string;
  int int_value;
  expression_t *right;
  char *op;
  expression_t *left;
  bool boolean_value;
  expression_t *condition;
  block_statement_t *consequence;
  block_statement_t *alternative;
  expression_t **parameters;
  int parameters_length;
  int parameters_capacity;
  block_statement_t *body;
} integer_literal_expression_t;

typedef struct boolean_literal_expression_t {
  node_type type;
  token_type token;
  Token_literal token_literal;
  Value value;
  Expression_node expression_node;
  String string;
  int int_value;
  expression_t *right;
  char *op;
  expression_t *left;
  bool boolean_value;
  expression_t *condition;
  block_statement_t *consequence;
  block_statement_t *alternative;
  expression_t **parameters;
  int parameters_length;
  int parameters_capacity;
  block_statement_t *body;
} boolean_literal_expression_t;

typedef struct prefix_expression_t {
  node_type type;
  token_type token;
  Token_literal token_literal;
  Value value;
  Expression_node expression_node;
  String string;
  int int_value;
  expression_t *right;
  char *op;
  expression_t *left;
  bool boolean_value;
  expression_t *condition;
  block_statement_t *consequence;
  block_statement_t *alternative;
  expression_t **parameters;
  int parameters_length;
  int parameters_capacity;
  block_statement_t *body;
} prefix_expression_t;

typedef struct infix_expression_t {
  node_type type;
  token_type token;
  Token_literal token_literal;
  Value value;
  Expression_node expression_node;
  String string;
  int int_value;
  expression_t *right;
  char *op;
  expression_t *left;
  bool boolean_value;
  expression_t *condition;
  block_statement_t *consequence;
  block_statement_t *alternative;
  expression_t **parameters;
  int parameters_length;
  int parameters_capacity;
  block_statement_t *body;
} infix_expression_t;

typedef struct if_expression_t {
  node_type type;
  token_type token;
  Token_literal token_literal;
  Value value;
  Expression_node expression_node;
  String string;
  int int_value;
  expression_t *right;
  char *op;
  expression_t *left;
  bool boolean_value;
  expression_t *condition;
  block_statement_t *consequence;
  block_statement_t *alternative;
  expression_t **parameters;
  int parameters_length;
  int parameters_capacity;
  block_statement_t *body;
} if_expression_t;

typedef struct function_expression_t {
  node_type type;
  token_type token;
  Token_literal token_literal;
  Value value;
  Expression_node expression_node;
  String string;
  int int_value;
  expression_t *right;
  char *op;
  expression_t *left;
  bool boolean_value;
  expression_t *condition;
  block_statement_t *consequence;
  block_statement_t *alternative;
  expression_t **parameters;
  int parameters_length;
  int parameters_capacity;
  block_statement_t *body;
} function_expression_t;

typedef expression_t *(*prefix_parse_function)();
typedef expression_t *(*infix_parse_function)();

expression_t *parse_identifier();
expression_t *parse_integer_literal();
expression_t *parse_prefix_expression();
expression_t *parse_boolean_expression();
expression_t *parse_grouped_expression();
expression_t *parse_if_expression();
expression_t *parse_function_expression();

expression_t *parse_infix_function();

static prefix_parse_function prefix_parse_functions[] = {
    NULL,
    NULL,
    parse_identifier,
    parse_integer_literal,
    NULL,
    NULL,
    parse_prefix_expression,
    parse_prefix_expression,
    NULL,
    NULL,
    parse_prefix_expression,
    parse_prefix_expression,
    NULL,
    NULL,
    parse_grouped_expression,
    NULL,
    NULL,
    NULL,
    parse_function_expression,
    NULL,
    parse_boolean_expression,
    parse_boolean_expression,
    parse_if_expression,
    NULL,
};

static infix_parse_function infix_parse_functions[] = {
    NULL, NULL, NULL, NULL, NULL,
    parse_infix_function, // SUM
    NULL,
    parse_infix_function, // SUM,
    parse_infix_function, // PRODUCT,
    parse_infix_function, // PRODUCT,
    parse_infix_function, // LESSGREATER,
    parse_infix_function, // LESSGREATER,

    // Delimiters
    NULL, NULL,

    NULL, NULL, NULL, NULL,

    // Keywords
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,

    parse_infix_function, // EQUALS,
    parse_infix_function, // NOTEQ
};
