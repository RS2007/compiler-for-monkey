#pragma once

#include "token.h"
#include <ctype.h>
#include <stdio.h>

typedef struct lexer_t
{
    // _t used to suffix all types
    char *input;
    size_t curr_position;
    size_t read_position;
    char ch;
} lexer_t;

lexer_t *new_lexer (char *, int);
void read_char_lexer (lexer_t *);
void trim_whitespace_lexer (lexer_t *);
token_t *next_token_lexer (lexer_t *);
char *read_identifier_lexer (lexer_t *);
char peek_char_lexer (lexer_t *);
void free_lexer (lexer_t *lexer);
