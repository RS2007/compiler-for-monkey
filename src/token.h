#pragma once

typedef enum token_type {
    ILLEGAL,
    END_OF_FILE,

    // Identifiers + Literals
    IDENT,
    INT,

    // Operators
    ASSIGN,
    PLUS,

    // Delimiters
    COMMA,
    SEMICOLON,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    // Keywords
    FUNCTION,
    LET
} token_type;

const char* token_strings[] = {
    "ILLEGAL",
    "END_OF_FILE",
    "IDENT",
    "INT",
    "ASSIGN",
    "PLUS",
    "COMMA",
    "SEMICOLON",
    "LPAREN",
    "RPAREN",
    "LBRACE",
    "RBRACE",
    "FUNCTION",
    "LET",
};

#define token_string_from_type(tok_type) token_strings[tok_type]
// tok_type since token_type is name of enum, not sure if needed

typedef struct token_t {
    token_type type;
    char* literal;
} token_t;