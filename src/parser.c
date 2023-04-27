#include "parser.h"
#include "nodes.h"
#include "token.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#define DEFAULT_DYNAMIC_ARR_SIZE 10
#define STRING_MAX_SIZE 1024
#define MAX_ERROR_SIZE 1024
#define unimplemented printf("\nUnimplemented\n");
#if 1
#define debug(...)                                                             \
  { printf(__VA_ARGS__); }
#else
#define debug(...)
#endif

char *print_program_string(void *program_node_cast_to_void) {
  program_t *program_node = ((program_t *)program_node_cast_to_void);
  char *program_node_string = (char *)malloc(32768 * sizeof(char));
  program_node_string[0] = '\0';
  for (int i = 0; i < program_node->statements_size; ++i) {
    statement_t *stmt = program_node->statements[i];
    char temp_string[512];
    sprintf(temp_string, "%s\n", stmt->string((void *)stmt));
    strcat(program_node_string, temp_string);
  }
  strcat(program_node_string, "\0");
  return program_node_string;
}

char *print_let_string(void *let_statement_cast_to_void) {
  let_statement_t *let_statement =
      ((let_statement_t *)let_statement_cast_to_void);
  char *let_node_string = (char *)malloc(STRING_MAX_SIZE);
  sprintf(let_node_string, "%s %s = %s", token_strings[let_statement->token],
          let_statement->iden_name, "unimplemented");
  return let_node_string;
}

char *print_return_string(void *ret_statement_cast_to_void) {
  ret_statement_t *ret_statement =
      ((ret_statement_t *)ret_statement_cast_to_void);
  char *ret_node_string = (char *)malloc(STRING_MAX_SIZE);
  sprintf(ret_node_string, "%s %s", token_strings[ret_statement->token],
          "unimplemented");
  return ret_node_string;
}

char *print_expression_string(void *expression_statement_cast_to_void) {
  expression_statement_t *expression_statement =
      (expression_statement_t *)expression_statement_cast_to_void;
  return expression_statement->expression->string(
      (void *)expression_statement->expression);
}

program_t *push_statements_program(program_t *program, statement_t *stmt) {
  if (program->statements_size + 1 > program->statements_capacity) {
    program->statements =
        realloc(program->statements, program->statements_capacity * 2);
  }
  program->statements[program->statements_size++] = stmt;
  return program;
}

parser_t *push_errors_parser(parser_t *parser, char *error_msg) {
  if (parser->errors_size + 1 > parser->errors_capacity) {
    parser->errors = realloc(parser->errors, parser->errors_capacity * 2);
  }
  parser->errors[parser->errors_size++] = error_msg;
  return parser;
}

parser_t *new_parser(lexer_t *lexer) {
  parser_t *parser = (parser_t *)malloc(sizeof(parser_t));
  parser->lexer = lexer;
  parser->errors = (char **)calloc(DEFAULT_DYNAMIC_ARR_SIZE, MAX_ERROR_SIZE);
  parser->errors_size = 0;
  parser->errors_capacity = DEFAULT_DYNAMIC_ARR_SIZE;
  // run twice to populate the curr_token and peek_token
  next_token_parser(parser);
  next_token_parser(parser);
  return parser;
}

void next_token_parser(parser_t *parser) {
  parser->curr_token = parser->peek_token;
  parser->peek_token = next_token_lexer(parser->lexer);
}

char *program_token_literal(void *program_cast_to_void) {
  program_t *program = (program_t *)program_cast_to_void;
  if (program->statements_size <= 0) {
    fprintf(stderr, "Fatal: program node has an invalid number of statements");
    exit(-1);
  }
  return program->statements[0]->token_literal(program->statements[0]);
}

char *let_token_literal(void *let_statement_cast_to_void) {
  let_statement_t *let_statement =
      (let_statement_t *)let_statement_cast_to_void;
  return token_strings[let_statement->token];
}

char *ret_token_literal(void *ret_statement_cast_to_void) {
  // TODO: is it possible to make a abstracted token literal function
  ret_statement_t *ret_statement =
      (ret_statement_t *)ret_statement_cast_to_void;
  return token_strings[ret_statement->token];
}

char *expression_token_literal(void *expression_statement_cast_to_void) {
  expression_statement_t *expression_statement =
      (expression_statement_t *)expression_statement_cast_to_void;
  return expression_statement->iden_name;
}

void statement_node_let(void *let_statement_cast_to_void) {
  let_statement_t *let_statement =
      (let_statement_t *)let_statement_cast_to_void;
  return;
}

program_t *new_program_node() {
  program_t *program_node = (program_t *)malloc(sizeof(program_t));
  if (program_node == NULL) {
    fprintf(stderr, "Fatal: Cannot allocate memory of %zu bytes",
            sizeof(program_t));
  }
  program_node->type = PROGRAM;
  program_node->token_literal = program_token_literal;
  // TODO: function pointer token_literal
  program_node->statements =
      (statement_t **)calloc(DEFAULT_DYNAMIC_ARR_SIZE, sizeof(statement_t));
  program_node->statements_size = 0;
  program_node->statements_capacity = DEFAULT_DYNAMIC_ARR_SIZE;
  program_node->string = print_program_string;
  // TODO: malloc statements and initialize array to 0 and capacity a macro
  // DEFAULT_DYNAMIC_ARR_SIZE
  return program_node;
}

bool is_curr_token(parser_t *parser, token_type type) {
  return parser->curr_token->type == type;
}

bool is_peek_token(parser_t *parser, token_type type) {
  return parser->peek_token->type == type;
}

bool expect_peek_token(parser_t *parser, token_type type) {
  /*
   * checks if peeked token is of the given type, if yes moves the parser to the
   * next token and returns true, if not just returns false without moving the
   * pointers
   */
  if (is_peek_token(parser, type)) {
    next_token_parser(parser);
    return true;
  }
  return false;
}

statement_t *parse_let_statement(parser_t *parser) {
  let_statement_t *stmt = (let_statement_t *)malloc(sizeof(let_statement_t));
  // 1. token info
  stmt->token = parser->curr_token->type;
  stmt->token_literal = let_token_literal;
  stmt->string = print_let_string;
  if (!expect_peek_token(parser, IDENT)) {
    char *error_string = (char *)malloc(MAX_ERROR_SIZE);
    snprintf(error_string, MAX_ERROR_SIZE, "Let is not followed by literal");
    fprintf(stderr, "Let is not followed by literal");
    push_errors_parser(parser, error_string);
    exit(-1);
    // FIXME: find a way to bubble errors and then print them in the root
    // program
  }
  // 2. identifier info
  stmt->iden_name = parser->curr_token->literal;
  if (!expect_peek_token(parser, ASSIGN)) {
    char *error_string = (char *)malloc(MAX_ERROR_SIZE);
    snprintf(error_string, MAX_ERROR_SIZE, "Expected token:%s ,Got: %s",
             token_strings[ASSIGN], token_strings[parser->peek_token->type]);
    printf("Expected token:%s ,Got: %s", token_strings[ASSIGN],
           token_strings[parser->peek_token->type]);
    push_errors_parser(parser, error_string);
    exit(-1);
    // FIXME: find a way to bubble errors and then print them in the root
    // program
  }
  while (!is_curr_token(parser, SEMICOLON)) {
    next_token_parser(parser);
  }
  return (statement_t *)stmt;
}
statement_t *parse_return_statement(parser_t *parser) {
  ret_statement_t *stmt = (ret_statement_t *)malloc(sizeof(ret_statement_t));
  stmt->token = parser->curr_token->type;
  stmt->token_literal = ret_token_literal;
  stmt->string = print_return_string;
  next_token_parser(parser);
  while (!is_curr_token(parser, SEMICOLON)) {
    next_token_parser(parser);
  }
  return (statement_t *)stmt;
}

expression_t *parse_expression(parser_t *parser) {
  prefix_parse_function prefix =
      prefix_parse_functions[parser->curr_token->type];
  // TODO: not sure about this check, might have to remove later
  if (prefix == NULL) {
    fprintf(stderr, "Prefix is null");
    exit(-1);
  }
  expression_t *left_expression = prefix(parser);
  return left_expression;
}

statement_t *parse_expression_statement(parser_t *parser) {
  expression_statement_t *stmt =
      (expression_statement_t *)malloc(sizeof(expression_statement_t));
  stmt->token = parser->curr_token->type;
  stmt->expression = parse_expression(parser);
  stmt->iden_name = parser->curr_token->literal;
  stmt->token_literal = expression_token_literal;
  if (is_peek_token(parser, SEMICOLON)) {
    next_token_parser(parser);
  }
  return (statement_t *)stmt;
}

expression_t *parse_identifier(parser_t *parser) {
  expression_t *expression =
      (expression_t *)malloc(sizeof(expression_statement_t));
  expression->token = parser->curr_token->type;
  expression->value = (Value)parser->curr_token->literal;
  return expression;
}

expression_t *parse_integer_literal(parser_t *parser) {
  expression_t *expression =
      (expression_t *)malloc(sizeof(expression_statement_t));
  expression->token = parser->curr_token->type;
  expression->int_value = atoi(parser->curr_token->literal);
  return expression;
}

statement_t *parse_statement(parser_t *parser) {
  switch (parser->curr_token->type) {
  case LET:
    return parse_let_statement(parser);
  case RETURN:
    return parse_return_statement(parser);
  default:
    return parse_expression_statement(parser);
  }
}

expression_t *parse_prefix_expression(parser_t *parser) {
  expression_t *expression =
      (expression_t *)malloc(sizeof(expression_statement_t));
  expression->token = parser->curr_token->type;
  expression->op = parser->curr_token->literal;
  next_token_parser(parser);
  expression->right = parse_expression(parser);
  return expression;
}

program_t *parse_program(parser_t *parser) {
  program_t *program_node = new_program_node();
  while (parser->curr_token->type != END_OF_FILE) {
    statement_t *stmt = parse_statement(parser);
    // TODO: there is a null check here, the api should be such that this doesn
    // t happen
    if (stmt != NULL) {
      push_statements_program(program_node, stmt);
    }
    next_token_parser(parser);
  }
  return program_node;
}
