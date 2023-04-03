#include "lexer.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexer_t* new_lexer(char* input, int input_size)
{
    lexer_t* lexer = malloc(sizeof(lexer_t));
    if (lexer == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", sizeof(lexer_t));
        abort();
    }
    lexer->input = strndup(input, input_size);
    lexer->curr_position = 0;
    lexer->read_position = 1;
    lexer->ch = input[0];
    // TODO: might have to check input size before access(throw error on empty file)
    return lexer;
}

void read_char_lexer(lexer_t* lexer)
{
    if (lexer->ch) {
        lexer->curr_position = lexer->read_position;
        lexer->read_position++;
        lexer->ch = lexer->input[lexer->curr_position];
    }
}

void trim_whitespace_lexer(lexer_t* lexer)
{
    while (lexer->ch && lexer->ch != '\t' && lexer->ch != '\n' && lexer->ch != ' ' && lexer->ch == '\r') {
        read_char_lexer(lexer);
    }
}

token_t* next_token_lexer(lexer_t* lexer)
{
    token_t* token = new_token();
    trim_whitespace_lexer(lexer);

    // switch to match character to token literals
    switch (lexer->ch) {
    case '=':
        token->literal = strdup("=");
        token->type = ASSIGN;
        read_char_lexer(lexer);
        break;
    case ';':
        token->literal = strdup(";");
        token->type = SEMICOLON;
        read_char_lexer(lexer);
        break;
    case '(':
        token->literal = strdup("(");
        token->type = LPAREN;
        read_char_lexer(lexer);
        break;
    case ')':
        token->literal = strdup(")");
        token->type = RPAREN;
        read_char_lexer(lexer);
        break;
    case ',':
        token->literal = strdup(",");
        token->type = COMMA;
        read_char_lexer(lexer);
        break;
    case '+':
        token->literal = strdup("+");
        token->type = PLUS;
        read_char_lexer(lexer);
        break;
    case '{':
        token->literal = strdup("{");
        token->type = LBRACE;
        read_char_lexer(lexer);
        break;
    case '}':
        token->literal = strdup("}");
        token->type = RBRACE;
        read_char_lexer(lexer);
        break;
    case 0:
        token->literal = "";
        token->type = END_OF_FILE;
    }
    return token;
}