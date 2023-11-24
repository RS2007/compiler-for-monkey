#pragma once

#include "lexer.h"
#include "nodes.h"

typedef struct parser_t
{
    lexer_t *lexer;
    token_t *curr_token;
    token_t *peek_token;
    char **errors;
    int errors_size;
    int errors_capacity;
} parser_t;

parser_t *new_parser (lexer_t *);
void next_token_parser (parser_t *);
program_t *parse_program (parser_t *);
program_t *new_program_node ();
void free_parser (parser_t *parser);
