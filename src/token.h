#pragma once
typedef enum token_type
{
    ILLEGAL,
    END_OF_FILE,

    // Identifiers + Literals
    IDENT,
    INT,

    // Operators
    ASSIGN,
    PLUS,
    BANG,
    MINUS,
    SLASH,
    ASTERIK,
    LT,
    GT,

    // Delimiters
    COMMA,
    SEMICOLON,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    // Keywords
    FUNCTION,
    LET,
    TRUE,
    FALSE,
    IF,
    ELSE,
    RETURN,

    // Equality
    EQ,
    NOT_EQ,
    // STRINGS
    STRING,
    // ARRAY
    LBRACKET,
    RBRACKET,
    // COLON
    COLON,
    LT_EQ,
    GT_EQ,
} token_type;

static const char *token_strings[]
    = { "ILLEGAL",  "END_OF_FILE", "IDENT",  "INT",     "ASSIGN",   "PLUS",
        "BANG",     "MINUS",       "SLASH",  "ASTERIK", "LT",       "GT",
        "COMMA",    "SEMICOLON",   "LPAREN", "RPAREN",  "LBRACE",   "RBRACE",
        "FUNCTION", "LET",         "TRUE",   "FALSE",   "IF",       "ELSE",
        "RETURN",   "EQ",          "NOT_EQ", "STRING",  "LBRACKET", "RBRACKET",
        "COLON",    "LT_EQ",       "GT_EQ" };

// #define token_string_from_type(tok_type) token_strings[tok_type]
//  tok_type since token_type is name of enum, not sure if needed

typedef struct token_t
{
    token_type type;
    char *literal;
} token_t;

token_t *new_token ();
token_type lookup_ident_token (char *);
