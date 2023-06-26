/* Copyright 2023 The Peppa Authors. */

#include "peppa/hash.h"

#include <stdlib.h>

#include "peppa/macros.h"
#include "peppa/alloc.h"

#define ROTL(x, r, n) (((x) << (r)) | ((x) >> ((n) - (r))))
#define ROTL32(x, r) ROTL(x, r, 32)
#define ROTL64(x, r) ROTL(x, r, 64)

#define FMIX32(h)   \
  h ^= h >> 16;     \
  h *= 0x85ebca6b;  \
  h ^= h >> 13;     \
  h *= 0xc2b2ae35;  \
  h ^= h >> 16

#define FMIX64(k)             \
  k ^= k >> 33;               \
  k *= 0xff51afd7ed558ccdULL; \
  k ^= k >> 33;               \
  k *= 0xc4ceb9fe1a85ec53ULL; \
  k ^= k >> 33

#define C1 UINT32_C(0xcc9e2d51)
#define C2 UINT32_C(0x1b873593)

#define K1 UINT64_C(0xc3a5c85c97cb3127)
#define K2 UINT64_C(0xb492b66fbe98f273)
#define K3 UINT64_C(0x9ae16a3b2f90404f)

struct HashContext {
  uint64_t seed;
  uint64_t hash;
  uint8_t state[16];
  int pos;
  uint64_t len;
};

/* Allocate an hash context. */
HashContext* allocHashContext() {
  return PPALLOC(HashContext);
}

void initHashContext(HashContext* context) {
  memset(context, 0, sizeof(*context));
}

void initHashContext2(HashContext* context, uint64_t seed) {
  initHashContext(context);
  context->seed = seed;
}

void updateHashContext(HashContext* context, const void* data, size_t size) {

}

uint64_t finishHashContext(HashContext* context) {

}

void freeHashContext(HashContext* context) {
  freeMemory(context);
}

void MurmurHash3_x64_128 ( const void * key, const int len,
                           const uint32_t seed, void * out )
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 16;
  int i;

  uint64_t h1 = seed;
  uint64_t h2 = seed;

  uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
  uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

  //----------
  // body

  const uint64_t * blocks = (const uint64_t *)(data);

  for(i = 0; i < nblocks; i++)
  {
    uint64_t k1 = getblock(blocks,i*2+0);
    uint64_t k2 = getblock(blocks,i*2+1);

    k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;

    h1 = ROTL64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

    k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

    h2 = ROTL64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
  }

  //----------
  // tail

  const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

  uint64_t k1 = 0;
  uint64_t k2 = 0;

  switch(len & 15)
  {
  case 15: k2 ^= (uint64_t)(tail[14]) << 48;
  case 14: k2 ^= (uint64_t)(tail[13]) << 40;
  case 13: k2 ^= (uint64_t)(tail[12]) << 32;
  case 12: k2 ^= (uint64_t)(tail[11]) << 24;
  case 11: k2 ^= (uint64_t)(tail[10]) << 16;
  case 10: k2 ^= (uint64_t)(tail[ 9]) << 8;
  case  9: k2 ^= (uint64_t)(tail[ 8]) << 0;
           k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

  case  8: k1 ^= (uint64_t)(tail[ 7]) << 56;
  case  7: k1 ^= (uint64_t)(tail[ 6]) << 48;
  case  6: k1 ^= (uint64_t)(tail[ 5]) << 40;
  case  5: k1 ^= (uint64_t)(tail[ 4]) << 32;
  case  4: k1 ^= (uint64_t)(tail[ 3]) << 24;
  case  3: k1 ^= (uint64_t)(tail[ 2]) << 16;
  case  2: k1 ^= (uint64_t)(tail[ 1]) << 8;
  case  1: k1 ^= (uint64_t)(tail[ 0]) << 0;
           k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len; h2 ^= len;

  h1 += h2;
  h2 += h1;

  h1 = fmix64(h1);
  h2 = fmix64(h2);

  h1 += h2;
  h2 += h1;

  ((uint64_t*)out)[0] = h1;
  ((uint64_t*)out)[1] = h2;
}
