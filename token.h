
#define STRING_MAX_SIZE 2000

enum TokenType {
  PLUS = 0,
  ILLEGAL = 1,
  END = 2,
  IDENT = 3,
  INT = 4,
  ASSIGN = 5,
  COMMA = 6,
  SEMICOLON = 7,
  LPAREN = 8,
  RPAREN = 9,
  LBRACE = 10,
  RBRACE = 11,
  FUNCTION = 12,
  LET = 13,
};

typedef struct Token {
  enum TokenType type;
  char literal;
} Token;

char *get_token_string(int index);
