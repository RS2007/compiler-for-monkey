#include <ctype.h>
#include <stdbool.h>
#include <string.h>

bool is_character(char ch) { return isalnum(ch) || ch == '_'; }

bool is_number(char *literal) {
  int size = strlen(literal);
  int i;
  for (i = 0; i < size; ++i) {
    if (!isdigit(literal[i])) {
      return false;
    }
  }
  return true;
}