#pragma once
#include <stdlib.h>

typedef enum precedeece {
  LOWEST,
  EQUALS,
  LESSGREATER,
  SUM,
  PRODUCT,
  PREFIX,
  CALL,
  INDEX
} precedence_t;

static precedence_t precedences_array[] = {
    LOWEST,
    LOWEST,
    LOWEST,
    LOWEST,
    LOWEST,
    SUM,
    LOWEST,
    SUM,
    PRODUCT,
    PRODUCT,
    LESSGREATER,
    LESSGREATER,

    // Delimiters
    LOWEST,
    LOWEST,

    CALL,
    LOWEST,
    LOWEST,
    LOWEST,

    // Keywords
    LOWEST,
    LOWEST,
    LOWEST,
    LOWEST,
    LOWEST,
    LOWEST,
    LOWEST,

    // Equality
    EQUALS,
    EQUALS,
    // STRINGS
    LOWEST,
    // ARRAY
    INDEX,
    LOWEST,
};
