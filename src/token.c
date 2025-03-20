#include "token.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_t *
new_token ()
{
    token_t *token = (token_t *)malloc (sizeof (token_t));
    if (token == NULL)
        {
            fprintf (stderr, "Fatal: failed to allocate %zu bytes.\n",
                     sizeof (token_t *));
            exit (-1);
        }
    return token;
}

token_type
lookup_ident_token (char *literal)
{

    if (strcmp (literal, "let") == 0)
        return LET;
    if (strcmp (literal, "fn") == 0)
        return FUNCTION;
    if (strcmp (literal, "true") == 0)
        return TRUE;
    if (strcmp (literal, "false") == 0)
        return FALSE;
    if (strcmp (literal, "if") == 0)
        return IF;
    if (strcmp (literal, "else") == 0)
        return ELSE;
    if (strcmp (literal, "return") == 0)
        return RETURN;
    if (is_number (literal))
        return INT;
    return IDENT;
}
