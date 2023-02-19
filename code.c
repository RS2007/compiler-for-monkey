#include <stdlib.h>
#include "hashmap.h"



enum CpCode {
    Op_constant
};

void setup_definitions(hashmap* hm){
    for(int i = Op_constant;i<=Op_constant;++i){
        opcode_defn* value = (opcode_defn*)malloc(sizeof(opcode_defn));
        byte* width = (byte*)calloc(2,sizeof(byte));
        value->string = "Op_constant";
        value->operandWidth = width;
        set_hashmap(hm,Op_constant,value);
    }
}

opcode_defn *lookup_opcode(hashmap* hm,byte key){
    return get_hashmap(hm,key);
}