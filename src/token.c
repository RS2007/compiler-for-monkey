#include "token.h"
#include <stdio.h>
#include <stdlib.h>

token_t* new_token()
{
    token_t* token = (token_t*)malloc(sizeof(token_t*));
    if (token == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", sizeof(token_t*));
        abort();
    }
    return token;
}