/* Copyright 2023 The Peppa Authors. */

#include "peppa/hash_table.h"

#include "peppa/hash.h"
#include "peppa/list.h"

struct Pe_Element {
  void* value;
  uint32_t hash_value;
  Pe_Node node;
};

struct Pe_HashTable {
  Pe_HashTableTraits traits;
};

Pe_HashTable* PeHashTable_alloc() {

}

void PeHashTable_init(Pe_HashTable* table, Pe_HashTableTraits traits) {

}

void PeHashTable_add() {

}

void PeHashTable_remove() {

}

int PeHashTable_find() {

}

void PeHashTable_release() {

}

void PeHashTable_free() {

}