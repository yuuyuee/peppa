/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_HASH_H_
#define PEPPA_HASH_H_

#include <stddef.h>
#include <stdint.h>

typedef struct  {
  uint32_t h1;
#define Pe_STATE_SIZE 4
  uint8_t state[Pe_STATE_SIZE];
  int pos;
  uint64_t len;
} Pe_HashContext;

/* Initialize or reinitialize an hash context. */
void PeHashContext_init(Pe_HashContext* context);

/* Initialize or reinitialize an hash context with a seed. */
void PeHashContext_init2(Pe_HashContext* context, uint32_t seed);

/* Update hash context with new data. */
void PeHashContext_update(Pe_HashContext* context,
                          const void* data, size_t len);

/* Finish hashing and returns hash value. */
uint32_t PeHashContext_finish(Pe_HashContext* context);

/* Wrapped functions */
uint32_t Pe_getHashValue(const void* data, size_t len);
uint32_t Pe_getHashValue2(const void* data, size_t len, uint32_t seed);

#endif  /* PEPPA_HASH_H_ */
