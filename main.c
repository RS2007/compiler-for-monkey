#include "code.h"
#include "lexer.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char code[] = "let five = 5;";
  Lexer *l = new_lexer(code);
  printf("%s", get_token_string(next_token(l)->type));
  return 0;
}
