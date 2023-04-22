#include "parser.h"
#include "token.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_DYNAMIC_ARR_SIZE 10
#define MAX_ERROR_SIZE 1024
#define unimplemented printf("\nUnimplemented\n");

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
  if (!expect_peek_token(parser, IDENT)) {
    char *error_string = (char *)malloc(MAX_ERROR_SIZE);
    snprintf(error_string, MAX_ERROR_SIZE, "Let is not followed by literal");
    fprintf(stderr,"Let is not followed by literal");
    push_errors_parser(parser, error_string);
    exit(-1);
    // FIXME: find a way to bubble errors and then print them in the root program
  }
  // 2. identifier info
  stmt->iden_name = parser->curr_token->literal;
  if (!expect_peek_token(parser, ASSIGN)) {
    char *error_string = (char *)malloc(MAX_ERROR_SIZE);
    snprintf(error_string, MAX_ERROR_SIZE, "Expected token:%s ,Got: %s",
             token_strings[ASSIGN], token_strings[parser->peek_token->type]);
    printf("Expected token:%s ,Got: %s",
             token_strings[ASSIGN], token_strings[parser->peek_token->type]);
    push_errors_parser(parser, error_string);
    exit(-1);
    // FIXME: find a way to bubble errors and then print them in the root program
  }
  while (!is_curr_token(parser, SEMICOLON)) {
    next_token_parser(parser);
  }
  return (statement_t *)stmt;
}

statement_t *parse_statement(parser_t *parser) {
  switch (parser->curr_token->type) {
  case LET:
    return parse_let_statement(parser);
  default:
    return NULL;
  }
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
