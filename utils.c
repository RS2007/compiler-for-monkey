#include <stdbool.h>
#include <string.h>
#include "lexer.h"

char array[14][STRING_MAX_SIZE] = {
        "+", "Illegal", "Eof", "Ident", "Int", "Assign", "Comma",
        "Semicolon", "Lparen", "Rparen", "LBrace", "Rbrace", "Function", "Let"};

bool isLetter(char input) {
    return (input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z');
}

int linear_search(int length, char *target) {
    // linear search by comparing target and strings in array
    for (int i = 0; i < length; i++) {
        if (strcmp(array[i], target) == 0) {
            return i;
        }
    }
    return -1;
}
