#include <stdio.h>
#include "code.h"

int main(int argc, char *argv[]) {
    hashmap* hm = create_hashmap();
    setup_definitions(hm);
    printf("%s",lookup_opcode(hm,0)->string);
    return 0;
}
