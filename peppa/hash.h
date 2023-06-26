/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_HASH_H_
#define PEPPA_HASH_H_

#include <stddef.h>
#include <stdint.h>

typedef struct HashContext HashContext;

/* Allocate an hash context. */
HashContext* allocHashContext();

/* Initialize or reinitialize an hash context. */
void initHashContext(HashContext* context);

/* Initialize or reinitialize an hash context with a seed. */
void initHashContext2(HashContext* context, uint64_t seed);

/* Update hash context with new data. */
void updateHashContext(HashContext* context, const void* data, size_t size);

/* Finish hashing and returns hash value. */
uint64_t finishHashContext(HashContext* context);

/* Free an hash context. */
void freeHashContext(HashContext* context);

/* Wrapped functions */

uint64_t getHashValue(const void* data, size_t size);
uint64_t getHashValue2(const void* data, size_t size, uint64_t seed);

#endif  /* PEPPA_HASH_H_ */
