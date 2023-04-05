#include "token.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_t* new_token()
{
    token_t* token = (token_t*)malloc(sizeof(token_t*));
    if (token == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", sizeof(token_t*));
        abort();
    }
    return token;
}

token_type lookup_ident_token(char* literal)
{

    if (strcmp(literal, "let") == 0)
        return LET;
    if (strcmp(literal, "fn") == 0)
        return FUNCTION;
    if (is_number(literal))
        return INT;
    return IDENT;
}