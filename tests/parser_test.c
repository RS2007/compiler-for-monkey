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

int main(void) {
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
}
