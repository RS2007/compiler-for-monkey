#include "hashmap.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int hash_function_opcode(Opcode *opcode)
{
    // square and take middle digits
    if (*opcode < 10)
    {
        printf("Invalid key in hashmap: %d", *opcode);
        exit(-1);
    }
    long long keySquared = (*opcode) * (*opcode);
    int digit_number = number_of_digits(keySquared);
    int middle;
    if (check_odd(digit_number))
    {
        middle = (digit_number + 1) / 2;
    }
    else
    {
        middle = (digit_number) / 2;
    }
    int ans = ((*opcode) / (exponent_int(10, middle))) % 10;
    return ans;
}
