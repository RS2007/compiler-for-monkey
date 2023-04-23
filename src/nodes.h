#pragma once

#include "token.h"

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
  Token_literal token_literal;
  Value value;
  Expression_node expression_node;
  String string;
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
  String string;
  // TODO: this field is not correct, fix this asap expression_type value;
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
} ret_statement_t;

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
} expression_statement_t;
