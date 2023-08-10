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

  /* Compares the two keys "x" and "y", returns nonzero ingeger if
   * it is equality. */
  int (*equal)(const void* x, const void* y);

  /* Hash function. */
  uint32_t (*hash)(const void* key);
} Pe_HashTableTraits;

typedef struct Pe_HashTable Pe_HashTable;

/* Allocate an hash table. */
Pe_HashTable* PeHashTable_alloc() Pe_NODISCARD;

/* Initialize an hash table with the traits. */
void PeHashTable_init(Pe_HashTable* table, Pe_HashTableTraits traits);

/* Add an element into the hash table. the element will be override
 * if the hash table does already contain an element with key equivalent
 * to key. */
void PeHashTable_add(Pe_HashTable* table, void* elt);

/* Remove an element from the hash table by the key of this element. */
void PeHashTable_remove(Pe_HashTable* table, const void* key);
void PeHashTable_remove2(Pe_HashTable* table, const void* elt);

/* Finds an element from the has table with key equivalent to key. */
void* PeHashTable_find(Pe_HashTable* table, const void* key);
void* PeHashTable_find2(Pe_HashTable* table, const void* elt);

typedef void (*Pe_HashTableVisitor)(const void* elt, void* opaque);

/* Applies the given function to the all elements of the hash table. */
void PeHashTable_foreach(Pe_HashTable* table,
                         Pe_HashTableVisitor visitor,
                         void* opaque);

/* Checks if the hash table has no elements. */
int PeHashTable_isEmpty(Pe_HashTable* table);

/* Clears all elements of the hash table. */
void PeHashTable_clear(Pe_HashTable* table);

/* Free an hash table. */
void PeHashTable_free(Pe_HashTable* table);

#endif  /* PEPPA_HASH_TABLE_H_ */
