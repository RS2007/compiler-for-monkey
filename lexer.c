//
// Created by rohith on 2/21/23.
//

#include "lexer.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

char A[14][STRING_MAX_SIZE] = {
        "+", "Illegal", "Eof", "Ident", "Int", "Assign", "Comma",
        "Semicolon", "Lparen", "Rparen", "LBrace", "Rbrace", "Function", "Let"};

Lexer *new_lexer(char *input) {
    Lexer *new_lexer_instance = (Lexer *) malloc(sizeof(Lexer));
    new_lexer_instance->input_string = input;
    new_lexer_instance->current_char = input[0];
    new_lexer_instance->read_position = 0;
    new_lexer_instance->position = 0;
    return new_lexer_instance;
}

void read_char(Lexer *l) {
    if (l->read_position >= strlen(l->input_string)) {
        l->current_char = 0;
    } else {
        l->current_char = l->input_string[l->read_position];
    }
    l->position = l->read_position;
    l->read_position += 1;
}

enum TokenType read_identifier(Lexer *l) {
    char* identifier = (char*)malloc(STRING_MAX_SIZE);
    int i = 0;
    while (true) {
        read_char(l);
        if (l->current_char == ' ') {
            break;
        }
        identifier[i++] =
                l->current_char; // increment current char and position in struct
    }
    identifier[0] = toupper(identifier[0]);
    return linear_search(14, identifier);
}

Token *next_token(Lexer *l) {
    Token *next_token_instance = (Token *) malloc(sizeof(Token));
    switch (l->current_char) {
        case '=':
            next_token_instance->type = PLUS;
            next_token_instance->literal = l->current_char;
            break;
        case ';':
            next_token_instance->type = SEMICOLON;
            next_token_instance->literal = l->current_char;
            break;
        case '(':
            next_token_instance->type = LPAREN;
            next_token_instance->literal = l->current_char;
            break;
        case ')':
            next_token_instance->type = RPAREN;
            next_token_instance->literal = l->current_char;
            break;
        case ',':
            next_token_instance->type = COMMA;
            next_token_instance->literal = l->current_char;
            break;
        case '+':
            next_token_instance->type = PLUS;
            next_token_instance->literal = l->current_char;
            break;
        case '{':
            next_token_instance->type = LBRACE;
            next_token_instance->literal = l->current_char;
            break;
        case '}':
            next_token_instance->type = RBRACE;
            next_token_instance->literal = l->current_char;
            break;
        case 0:
            next_token_instance->type = END;
            next_token_instance->literal = '\n';
            break;
        default:
            if (isLetter(l->current_char)) {
                next_token_instance->type = read_identifier(l);
            }
    }
    read_char(l);
    return next_token_instance;
}
