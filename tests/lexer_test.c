#include "../src/lexer.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    char* input = "let five = 5;"
                  "let ten = 10;"
                  "let add = fn(x, y){"
                  "x + y;"
                  "}"
                  "let result = add(five, ten);";
    const char* results[]
        = {
              "LET",
              "IDENT",
              "ASSIGN",
              "INT",
              "SEMICOLON",
              "LET",
              "IDENT",
              "ASSIGN",
              "INT",
              "SEMICOLON",
              "LET",
              "IDENT",
              "ASSIGN",
              "FUNCTION",
              "LPAREN",
              "IDENT",
              "COMMA",
              "IDENT",
              "RPAREN",
              "LBRACE",
              "IDENT",
              "PLUS",
              "IDENT",
              "SEMICOLON",
              "RBRACE",
              "LET",
              "IDENT",
              "ASSIGN",
              "IDENT",
              "LPAREN",
              "IDENT",
              "COMMA",
              "IDENT",
              "RPAREN",
              "SEMICOLON",
              "END_OF_FILE"
          };

    int input_size = strlen(input);
    lexer_t* lexer = new_lexer(input, input_size);
    int i = 0;
    int fail = 0;
    while (1) {
        token_t* current_token = next_token_lexer(lexer);
        if (current_token->type == END_OF_FILE) {
            break;
        }
        if (token_strings[current_token->type] != results[i]) {
            fprintf(stderr, "Test failed, expected: %s, got: %s\n", results[i], token_strings[current_token->type]);
            fail = 1;
            break;
        }
        ++i;
    }
    if (!fail) {
        fprintf(stdout, "All Tests Passed");
    }
    return 0;
}
