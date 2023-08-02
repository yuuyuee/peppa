/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_HASH_H_
#define PEPPA_HASH_H_

#include <stddef.h>
#include <stdint.h>

typedef uint32_t Pe_HashValue;
typedef struct Pe_HashContext Pe_HashContext;

/* Allocate an hash context. */
Pe_HashContext* PeHashContext_alloc();

/* Initialize or reinitialize an hash context. */
void PeHashContext_init(Pe_HashContext* context);

/* Initialize or reinitialize an hash context with a seed. */
void PeHashContext_init2(Pe_HashContext* context, Pe_HashValue seed);

/* Update hash context with new data. */
void PeHashContext_update(Pe_HashContext* context,
                          const void* data, size_t len);

/* Finish hashing and returns hash value. */
Pe_HashValue PeHashContext_finish(Pe_HashContext* context);

/* Free an hash context. */
void PeHashContext_free(Pe_HashContext* context);

/* Wrapped functions */

Pe_HashValue Pe_getHashValue(const void* data, size_t len);
Pe_HashValue Pe_getHashValue2(const void* data, size_t len, Pe_HashValue seed);

#endif  /* PEPPA_HASH_H_ */
