#include "../src/lexer.h"
#include <stdio.h>

int main(void)
{
    const char* input = "=+(){},;";
    const char* results[] = {
        "ASSIGN",
        "PLUS",
        "LPAREN",
        "RPAREN",
        "LBRACE",
        "RBRACE",
        "COMMA",
        "SEMICOLON"
    };
    int input_size
        = sizeof(input) / sizeof(input[0]);
    lexer_t* lexer = new_lexer(input, input_size);
    int i = 0;
    int fail = 0;
    while (1) {
        token_t* current_token = next_token_lexer(lexer);
        if (current_token->type == END_OF_FILE) {
            break;
        }
        if (token_strings[current_token->type] != results[i]) {
            fprintf(stderr, "Test failed, expected: %s, got: %s", results[i], token_strings[current_token->type]);
            fail = 1;
        }
        ++i;
    }
    if (!fail) {
        fprintf(stdout, "Test Passed");
    }
    return 0;
}
