#include<stdint.h>

typedef int8_t byte;


typedef struct Definition {
    char* Name;
    int* OperandWidths;
} Definition;

typedef byte* Instructions;
typedef byte Opcode;

