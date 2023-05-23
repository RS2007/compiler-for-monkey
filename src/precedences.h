#pragma once
#include <stdlib.h>

typedef enum precedeece {
  LOWEST,
  EQUALS,
  LESSGREATER,
  SUM,
  PRODUCT,
  PREFIX,
} precedence_t;

static precedence_t precedences_array[] = {
    LOWEST, LOWEST, LOWEST, LOWEST, LOWEST, SUM, LOWEST, SUM, PRODUCT, PRODUCT,
    LESSGREATER, LESSGREATER,

    // Delimiters
    LOWEST, LOWEST,

    LOWEST, LOWEST, LOWEST, LOWEST,

    // Keywords
    LOWEST, LOWEST, LOWEST, LOWEST, LOWEST, LOWEST, LOWEST,

    EQUALS, EQUALS

};
