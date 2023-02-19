#include <stdbool.h>
#include <stdint.h>

#define HASHMAP_CAPACITY 1000
typedef uint8_t byte;
/**
 * Implementation of hashmap with linear probing
 */

typedef struct opcode_defn
{
  char *string;
  byte *operandWidth;
} opcode_defn;

typedef struct hashmap_item
{
  byte key;
  opcode_defn *value;
  bool is_occupied;
} hashmap_item;

typedef struct hashmap
{
  // something like a dynamic array
  hashmap_item **items;
  int capacity;
  int length;
} hashmap;

unsigned int hash_function_opcode(byte opcode);
hashmap *create_hashmap();
void set_hashmap(hashmap *hashmap_to_be_updated, byte key,
                 opcode_defn *value);
void debug_hashmap(hashmap *hm);
opcode_defn *get_hashmap(hashmap *hm, byte key);