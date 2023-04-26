#include "../src/parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATEMENTS_NUMBER 3

bool test_let_statement(statement_t *ast_statement, char *name) {
  char *token_literal = ast_statement->token_literal((void *)ast_statement);
  if (strcmp(token_literal, "LET") != 0) {
    fprintf(stderr, "Expected: let statement, Got: %s", token_literal);
    return false;
  }
  let_statement_t *let_statement = (let_statement_t *)ast_statement;
  if (strcmp(name, let_statement->iden_name) != 0) {
    fprintf(stderr, "Expected: name of literal: %s, got %s", name,
            let_statement->iden_name);
    return false;
  }
  return true;
}

int test_let_statements(void) {
  char *input = "let x = 5;"
                "let y  = 10;"
                "let foobar = 838383;";
  lexer_t *lexer = new_lexer(input, strlen(input));
  parser_t *parser = new_parser(lexer);
  program_t *program_node = parse_program(parser);
  if (parser->errors_size != 0) {
    int i;
    for (i = 0; i < parser->errors_size; ++i) {
      fprintf(stderr, "%s\n", parser->errors[i]);
    }
    exit(-1);
  }
  if (program_node->statements_size != STATEMENTS_NUMBER) {
    fprintf(stderr, "Error: expected %d statements, got %d statements",
            STATEMENTS_NUMBER, program_node->statements_size);
    return 69;
  }
  char *expectedIdentifiers[] = {"x", "y", "foobar"};
  int i;
  for (i = 0; i < STATEMENTS_NUMBER; ++i) {
    statement_t *stmt = program_node->statements[i];
    if (!test_let_statement(stmt, expectedIdentifiers[i])) {
      // fprintf(stderr,"Expected: %s,got:
      // %s",expectedIdentifiers[i],((let_statement_t*)stmt)->iden_name);
      fprintf(stderr, "Hello this is an error");
    }
  }
  fprintf(stdout, "All tests passed");
  return 0;
}

int test_ret_statements(void) {
  char *input = "return 5;"
                "return 10;"
                "return 993322;";
  lexer_t *lexer = new_lexer(input, strlen(input));
  parser_t *parser = new_parser(lexer);
  program_t *program_node = parse_program(parser);
  if (parser->errors_size != 0) {
    int i;
    for (i = 0; i < parser->errors_size; ++i) {
      fprintf(stderr, "%s\n", parser->errors[i]);
    }
    exit(-1);
  }
  if (program_node->statements_size != STATEMENTS_NUMBER) {
    fprintf(stderr, "Error: expected %d statements, got %d statements",
            STATEMENTS_NUMBER, program_node->statements_size);
    return 69;
  }
  int i;
  for (i = 0; i < STATEMENTS_NUMBER; ++i) {
    statement_t *stmt = program_node->statements[i];
    if (strcmp(stmt->token_literal((void *)stmt), "RETURN") != 0) {
      fprintf(stderr, "Token literal not RETURN");
      return 69;
    }
  }
  fprintf(stdout, "All tests passed");
  return 0;
}

int test_string_method_on_statements(void) {
  char *input = "return 5;"
                "let a = 5;"
                "return 993322;";
  lexer_t *lexer = new_lexer(input, strlen(input));
  parser_t *parser = new_parser(lexer);
  program_t *program_node = parse_program(parser);
  printf("%s", program_node->string((void *)program_node));
  return 0;
}

int test_parsing_identifiers(void) {
  char *input = "foobar;";
  lexer_t *lexer = new_lexer(input, strlen(input));
  parser_t *parser = new_parser(lexer);
  program_t *program_node = parse_program(parser);
  int i;
  if (program_node->statements_size != 1) {
    fprintf(stderr, "Expected %d statements , got %d statements", 1,
            program_node->statements_size);
    exit(-1);
  }

  if (strcmp(program_node->statements[0]->iden_name, "foobar") != 0) {
    fprintf(stderr, "Expected identifier name is %s, got %s", "foobar",
            program_node->statements[0]->iden_name);
    exit(-1);
  }

  if (strcmp(program_node->statements[0]->token_literal(
                 (void *)program_node->statements[0]),
             "foobar") != 0) {
    fprintf(stderr, "Expected identifier name is %s, got %s", "foobar",
            program_node->statements[0]->token_literal(
                (void *)program_node->statements[0]));
    exit(-1);
  }
  printf("All tests passed");
  return 0;
}

int main(void) {
  char *input = "5;";
  lexer_t *lexer = new_lexer(input, strlen(input));
  parser_t *parser = new_parser(lexer);
  program_t *program_node = parse_program(parser);
  if (program_node->statements_size != 1) {
    fprintf(stderr, "Expected %d statements , got %d statements", 1,
            program_node->statements_size);
    exit(-1);
  }
  if (strcmp(program_node->statements[0]->token_literal(
                 (void *)program_node->statements[0]),
             "5") != 0) {
    fprintf(stderr, "Expected identifier name is %s, got %s", "5",
            program_node->statements[0]->token_literal(
                (void *)program_node->statements[0]));
    exit(-1);
  }
  if (program_node->statements[0]->expression->int_value != 5) {
    fprintf(stderr, "Expected integer value is %d got %d", 5,
            program_node->statements[0]->expression->int_value);
    exit(-1);
  }

  printf("All tests passed");
  return 0;
}
