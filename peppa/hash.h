/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_HASH_H_
#define PEPPA_HASH_H_

#include <stddef.h>
#include <stdint.h>

typedef struct PeHash_Context PeHash_Context;

/* Allocate an hash context. */
PeHash_Context* PeHash_alloc();

/* Initialize or reinitialize an hash context. */
void PeHash_init(PeHash_Context* context);

/* Initialize or reinitialize an hash context with a seed. */
void PeHash_init2(PeHash_Context* context, uint32_t seed);

/* Update hash context with new data. */
void PeHash_update(PeHash_Context* context, const void* data, size_t len);

/* Finish hashing and returns hash value. */
uint32_t PeHash_finish(PeHash_Context* context);

/* Free an hash context. */
void PeHash_free(PeHash_Context* context);

/* Wrapped functions */

uint32_t PeHash_getHashValue(const void* data, size_t len);
uint32_t PeHash_getHashValue2(const void* data, size_t len, uint32_t seed);

#endif  /* PEPPA_HASH_H_ */
