#include "lexer.h"
#include <stdio.h>
#include <string.h>

#define MAX_STRING_SIZE 1000

int main(void)
{
    char user_input[MAX_STRING_SIZE];
    printf(">>");
    fgets(user_input, MAX_STRING_SIZE, stdin);
    lexer_t* lexer
        = new_lexer(user_input, strlen(user_input));
    while (1) {
        token_t* current_token = next_token_lexer(lexer);
        if (current_token->type == END_OF_FILE) {
            break;
        }
        fprintf(stdout, "%s\n", token_strings[current_token->type]);
    }
}