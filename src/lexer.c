#include "lexer.h"
#include "string.h"
#include "utils.h"
#include <ctype.h>
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
    while (lexer->ch && (lexer->ch == '\t' || lexer->ch == '\n' || lexer->ch == ' ' || lexer->ch == '\r')) {
        read_char_lexer(lexer);
    }
}

char* read_identifier_lexer(lexer_t* lexer)
{
    int curr_position = lexer->curr_position;
    while (is_character(lexer->input[lexer->curr_position])) {
        lexer->curr_position++;
    }
    size_t word_size = lexer->curr_position - curr_position;
    char* identifier = (char*)malloc(word_size + 1);
    memcpy(identifier, lexer->input + curr_position, word_size);
    identifier[word_size] = '\0';
    lexer->read_position = lexer->curr_position + 1;
    lexer->ch = lexer->input[lexer->curr_position];
    return identifier;
}

char peek_char_lexer(lexer_t* lexer)
{
    if (strlen(lexer->input) <= lexer->read_position) {
        fprintf(stderr, "Cannot peek beyond input");
        abort();
    }
    char peek_char = lexer->input[lexer->read_position];
    return peek_char;
}

token_t* next_token_lexer(lexer_t* lexer)
{
    token_t* token = new_token();
    trim_whitespace_lexer(lexer);

    // switch to match character to token literals
    switch (lexer->ch) {
    case '=':
        if (peek_char_lexer(lexer) == '=') {
            token->literal = strdup("==");
            token->type = EQ;
            // TODO: refactor with jump_char_lexer(instead of calling read_char_lexer twice)
            read_char_lexer(lexer);
            read_char_lexer(lexer);
            break;
        }
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
    case '!':
        if (peek_char_lexer(lexer) == '=') {
            token->literal = strdup("!=");
            token->type = NOT_EQ;
            // TODO: refactor with jump_char_lexer(instead of calling read_char_lexer twice)
            read_char_lexer(lexer);
            read_char_lexer(lexer);
            break;
        }
        token->literal = strdup("!");
        token->type = BANG;
        read_char_lexer(lexer);
        break;
    case '-':
        token->literal = strdup("-");
        token->type = MINUS;
        read_char_lexer(lexer);
        break;
    case '/':
        token->literal = strdup("/");
        token->type = SLASH;
        read_char_lexer(lexer);
        break;
    case '*':
        token->literal = strdup("*");
        token->type = ASTERIK;
        read_char_lexer(lexer);
        break;
    case '<':
        token->literal = strdup("<");
        token->type = LT;
        read_char_lexer(lexer);
        break;
    case '>':
        token->literal = strdup(">");
        token->type = GT;
        read_char_lexer(lexer);
        break;
    case '\0':
        token->literal = "";
        token->type = END_OF_FILE;
        break;
    default:
        if (is_character(lexer->ch)) {
            token->literal = read_identifier_lexer(lexer);
            token->type = lookup_ident_token(token->literal);
            return token;
        }
        token->type = ILLEGAL;
        token->literal = strdup(&lexer->ch);
    }
    return token;
}