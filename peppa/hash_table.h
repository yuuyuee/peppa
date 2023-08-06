/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_HASH_TABLE_H_
#define PEPPA_HASH_TABLE_H_

#include <stdint.h>
#include <stddef.h>

typedef struct Pe_HashTableTraits {
  /* Compares the objects and returns 0 if it is one in the same. */
  int (*compare)(const void* left_elem, const void* right_elem);

  /* Takes object which pointed into "ptr" and returns key. */
  const void* (*extractKey)(const void* elem);

  /* Hash function. */
  uint32_t (*hash)(const void* key);
} Pe_HashTableTraits;

typedef struct Pe_HashTableNode {
  void* value;
  uint32_t hash_value;
  Pe_HashTableNode* next;
} Pe_HashTableNode;

struct Pe_HashTable {
  Pe_HashTableTraits traits;
  Pe_HashTableNode* nodes;
  size_t num_buckets;
  size_t num_elements;
  float load_factor;
};


typedef struct Pe_HashTable Pe_HashTable;

void PeHashTable_init(Pe_HashTable* table, Pe_HashTableTraits traits);

void PeHashTable_add(Pe_HashTable* table, void* elem);

void PeHashTable_remove(Pe_HashTable* table, const void* key);

int PeHashTable_find(Pe_HashTable* table, const void* key);

typedef void (*Pe_HashTableVisitor)(const void* elem, void* opaque);
void PeHashTable_foreach(Pe_HashTable* table,
                         Pe_HashTableVisitor visitor,
                         void* opaque);

void PeHashTable_destory();

#endif  /* PEPPA_HASH_TABLE_H_ */
