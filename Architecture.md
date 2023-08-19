# Design generic data structures for monkey

## What does generic mean?

- The data structure should be agnostic:
  > For example the function `create_data_structure()` should return the same
  > type at all times, there can be a type field within the data structure to keep
  > track of its type
- types should be enums(at all points the project maintainer should be able to
  list all the types that can be used in a generic data structure)

### Designing Linked List

- Linked list should have a single type argument, which is the type of the data
  contained within a linked list node.

### Designing HashMap

- HashMap is an ADT with a static array of linked list heads. Hashmap takes two
  generic arguments a key type and a value type
- but since linked list is a dependency for the hashmap, what should be passed
  in as the argument to the linked list
  - a key value pair has to be passed in, cause the key info is lost while hashing
    and unhashing at all times is unnecessary compute.
  - create an auxillary data type to be passed into the linked list which contains
    the key and value pair
    - This auxillary data type should also be generic and hence should hold the key
      and value type which is obtained from the hashmap

## Refactor

- Where are hashmaps being used?
  - Tests
  - [x] Environment
  - [x] Hash Literal(Parsing)
  - [ ] Hash Literal(Evaluation)
  - [x] Inspect Hash Literal(Object)

### Problem with hashKey hashmap

- should we store hashkey and
