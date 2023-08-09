#include "environment.h"
#include "evaluator.h"
#include "lexer.h"
#include "object.h"
#include "parser.h"
#include "utils.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quit() {
  printf("\nExiting from the repl....\n");
  exit(0);
}

void repl() {
  signal(SIGINT, quit);
  environment_t *env = new_environment();
  while (1) {
    char user_input[STRING_MAX_SIZE];
    user_input[0] = '\0';
    printf(">>>");
    fgets(user_input, STRING_MAX_SIZE, stdin);
    if (strcmp(user_input, "quit\n") == 0) {
      quit();
    }
    lexer_t *lexer = new_lexer(user_input, strlen(user_input));
    parser_t *parser = new_parser(lexer);
    program_t *program_node = parse_program(parser);
    int has_error = 0;
    for (int i = 0; i < parser->errors_size; ++i) {
      char *error_string = parser->errors[i];
      printf("Parser error: %s\n", error_string);
      has_error = 1;
    }
    if (has_error) {
      continue;
    }
    object_t *object = eval((node_t *)program_node, env);
    if (object == NULL) {
      continue;
    }
    if (object->type((void *)object) == ERROR_OBJ) {
      error_obj_t *error_obj = (error_obj_t *)object;
      printf("Evaluator error: %s\n", error_obj->message);
      continue;
    }
    printf("%s\n", object->inspect((void *)object));
    free_object(object);
    free_lexer(lexer);
    free_parser(parser);
    for (int i = 0; i < STRING_MAX_SIZE; ++i) {
      user_input[i] = '\0';
    }
  }
}
