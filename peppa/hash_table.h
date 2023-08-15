/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_HASH_TABLE_H_
#define PEPPA_HASH_TABLE_H_

#include <stdint.h>
#include <stddef.h>

#include "peppa/attributes.h"

typedef struct Pe_HashTableTraits {
  /* Takes the key of the elements. */
  const void* (*extract)(const void* elt);

  /* Free an element. */
  void (*free)(void* elt);

  /* Compares the two keys of "x" and "y", returns zero if it is equality. */
  int (*compare)(const void* x, const void* y);

  /* Hash function. */
  uint32_t (*hash)(const void* key);
} Pe_HashTableTraits;

typedef struct Pe_HashTable Pe_HashTable;

/* Allocate an hash table. */
Pe_HashTable* PeHashTable_alloc() Pe_NODISCARD;

/* Initialize an hash table with the traits. */
void PeHashTable_init(Pe_HashTable* table, Pe_HashTableTraits traits, size_t n);
void PeHashTable_init2(Pe_HashTable* table, Pe_HashTableTraits traits);

/* Inserts an element into the hash table. return zero if the hash table
 * does not already contain an element with key equivalent to key,
 * otherwise returns nonezero value. */
int PeHashTable_insert(Pe_HashTable* table, void* elt);

/* Removes an element from the hash table by the key of this element. */
void PeHashTable_remove(Pe_HashTable* table, const void* key);
void PeHashTable_remove2(Pe_HashTable* table, const void* elt);

/* Finds an element from the has table with key equivalent to key. */
void* PeHashTable_find(Pe_HashTable* table, const void* key);
void* PeHashTable_find2(Pe_HashTable* table, const void* elt);

typedef void (*Pe_HashTableVisitor)(const void* elt, void* opaque);

/* Applies the given function to the all elements of the hash table. */
void PeHashTable_forEach(const Pe_HashTable* table,
                         Pe_HashTableVisitor visitor,
                         void* opaque);

/* Checks if the hash table has no elements. */
int PeHashTable_isEmpty(const Pe_HashTable* table);

/* Clears all elements in the hash table. */
void PeHashTable_clear(Pe_HashTable* table);

/* Returns the number of elements in the hash table. */
size_t PeHashTable_getSize(const Pe_HashTable* table);

/* Returns the number of buckets in the hash table. */
size_t PeHashTable_getBucketSize(const Pe_HashTable* table);

/* Returns the average number of elements per bucket. */
float PeHashTable_getLoadFactor(const Pe_HashTable* table);

/* Frees an hash table. */
void PeHashTable_free(Pe_HashTable* table);

#endif  /* PEPPA_HASH_TABLE_H_ */
