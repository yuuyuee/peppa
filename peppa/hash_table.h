/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_HASH_TABLE_H_
#define PEPPA_HASH_TABLE_H_

#include "peppa/hash.h"
#include "peppa/attributes.h"

typedef struct Pe_HashTableTraits {
  /* Compares the objects and returns 0 if it is one in the same. */
  int (*compare)(const void* lobj, const void* robj);

  /* Takes object which pointed into "ptr" and returns key. */
  const void* (*extractKey)(const void* obj);

  /* Hash function. */
  Pe_HashValue (*hash)(const void* key);
} Pe_HashTableTraits;

typedef struct Pe_HashTable Pe_HashTable;

Pe_HashTable* PeHashTable_alloc() Pe_NODISCARD;

void PeHashTable_init(Pe_HashTable* table, Pe_HashTableTraits traits);

void PeHashTable_add();

void PeHashTable_remove();

int PeHashTable_find();

void PeHashTable_release();

void PeHashTable_free();

#endif  /* PEPPA_HASH_TABLE_H_ */
