/* Copyright 2023 The Peppa Authors. */

#include "peppa/hash_table.h"

#include "peppa/alloc.h"
#include "peppa/macros.h"

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

Pe_HashTable* PeHashTable_alloc() {
  return Pe_CAST(Pe_HashTable*, Pe_alloc(sizeof(Pe_HashTable)));
}

void PeHashTable_init(Pe_HashTable* table, Pe_HashTableTraits traits) {

}

void PeHashTable_add(Pe_HashTable* table, void* elem) {

}

void PeHashTable_remove(Pe_HashTable* table, const void* key) {

}

int PeHashTable_find(Pe_HashTable* table, const void* key) {

}

void PeHashTable_foreach(Pe_HashTable* table,
                         Pe_HashTableVisitor visitor,
                         void* opaque) {

}

void PeHashTable_free(Pe_HashTable* table) {

}
