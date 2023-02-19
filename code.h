#include <stdint.h>
#include "hashmap.h"


typedef struct Definition
{
    char *Name;
    int *OperandWidths;
} Definition;

typedef byte *Instructions;



void setup_definitions(hashmap* hm);
opcode_defn * lookup_opcode(hashmap* hm,byte key);
