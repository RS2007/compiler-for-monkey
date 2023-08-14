#include "environment.h"
#include "evaluator.h"
#include "lexer.h"
#include "parser.h"
#include "repl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE *file;
  char *buffer;
  long file_size;
  if (argc < 2) {
    repl();
    return 0;
  }
  char *extension = strchr(argv[1], '.');
  if (!(extension && (strcmp(extension, ".monkey") == 0))) {
    fprintf(stderr, "Monkey files should end with a .monkey extension\n");
    exit(1);
  }
  file = fopen(argv[1], "r");
  if (file == NULL) {
    fprintf(stderr, "Error opening file");
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  rewind(file);
  buffer = (char *)malloc(file_size + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Error allocating memory");
    exit(1);
  }
  fread(buffer, 1, file_size, file);
  buffer[file_size] = '\0';
  environment_t *env = new_environment();
  lexer_t *lexer = new_lexer(buffer, strlen(buffer));
  parser_t *parser = new_parser(lexer);
  program_t *program_node = parse_program(parser);
  int has_error = 0;
  for (int i = 0; i < parser->errors_size; ++i) {
    char *error_string = parser->errors[i];
    printf("Parser error: %s\n", error_string);
    has_error = 1;
  }
  if (has_error) {
    exit(1);
  }
  object_t *object = eval((node_t *)program_node, env);
  if (object == NULL) {
    exit(1);
  }
  if (object->type((void *)object) == ERROR_OBJ) {
    error_obj_t *error_obj = (error_obj_t *)object;
    printf("Evaluator error: %s\n", error_obj->message);
    exit(1);
  }
  printf("%s\n", object->inspect((void *)object));
  free_object(object);
  free_lexer(lexer);
  free_parser(parser);
  for (int i = 0; i < STRING_MAX_SIZE; ++i) {
    buffer[i] = '\0';
  }
  free(buffer);
  return 0;
}
