#include "token.h"

#define STRING_MAX_SIZE 2000

static char array[STRING_MAX_SIZE][14] = {"+", "Illegal", "Eof", "Ident", "Int", "Assign", "Comma", "Semicolon", "Lparen", "Rparen", "LBrace", "Rbrace", "Function", "Let"};


char* get_token_string(int index){
    return array[index];
}