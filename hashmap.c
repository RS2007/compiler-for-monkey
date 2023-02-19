#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "code.h"


unsigned int hash_function_opcode(byte opcode) {
    // from
    // https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
    int hashed_opcode = ((opcode >> 16) ^ opcode) * 0x45d9f3b;
    hashed_opcode = ((hashed_opcode >> 16) ^ hashed_opcode) * 0x45d9f3b;
    hashed_opcode = (hashed_opcode >> 16) ^ hashed_opcode;
    return abs(hashed_opcode);
}

unsigned int find_slot_hashmap(hashmap *hashmap_to_be_searched, byte opcode) {
    unsigned int key_index =
            hash_function_opcode(opcode) % (hashmap_to_be_searched->capacity);
    while (hashmap_to_be_searched->items[key_index]->is_occupied &&
           hashmap_to_be_searched->items[key_index]->key != opcode) {
        key_index = (key_index + 1) % hashmap_to_be_searched->length;
    }
    return key_index;
}

hashmap *create_hashmap() {
    hashmap *new_hashmap = (hashmap *) malloc(sizeof(hashmap));
    new_hashmap->items = (hashmap_item **) calloc(HASHMAP_CAPACITY, sizeof(hashmap_item));
    for (int i = 0; i < HASHMAP_CAPACITY; ++i) {
        new_hashmap->items[i] = (hashmap_item *) malloc(sizeof(hashmap_item));
        new_hashmap->items[i]->is_occupied = false;
    }
    new_hashmap->capacity = HASHMAP_CAPACITY;
    new_hashmap->length = 1;
    return new_hashmap;
}

void set_hashmap(hashmap *hashmap_to_be_updated, byte key, opcode_defn *value) {
    int key_index = find_slot_hashmap(hashmap_to_be_updated, key);
    if (hashmap_to_be_updated->capacity < hashmap_to_be_updated->length + 1) {
        hashmap_to_be_updated->capacity *= 2;
        hashmap_to_be_updated = realloc(hashmap_to_be_updated->items,
                                        sizeof(hashmap_to_be_updated->items) * 2);
    }
    if (!hashmap_to_be_updated->items[key_index]->is_occupied) {
        hashmap_to_be_updated->items[key_index]->key = key;
        hashmap_to_be_updated->items[key_index]->value = value;
        hashmap_to_be_updated->items[key_index]->is_occupied = true;
        return;
    }
}

opcode_defn * get_hashmap(hashmap* hm,byte key){
    unsigned int key_index = find_slot_hashmap(hm,key);
    if(hm->items[key_index]->is_occupied){
        return hm->items[key_index]->value;
    }else{
        return NULL;
    }
}

void debug_hashmap(hashmap *hm) {
    for (int i = 0; i < hm->capacity; ++i) {
        if (hm->items[i]->is_occupied) {
            printf("Key: %d,Value: %s\n", hm->items[i]->key,
                   hm->items[i]->value->string);
        }
    }
}
