//
// Created by rohith on 2/21/23.
//
#include "token.h"
#include <string.h>

typedef struct Lexer {
  char *input_string;
  int position;      // current char
  int read_position; // next char to read
  char current_char;
} Lexer;

Lexer *new_lexer(char *input);
void read_char(Lexer *l);
Token *next_token(Lexer *l);
