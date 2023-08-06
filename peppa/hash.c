/* Copyright 2023 The Peppa Authors. */

#include "peppa/hash.h"

#include <string.h>

#include "peppa/codec.h"
#include "peppa/attributes.h"

#define _Pe_ROTL(x, r, n) (((x) << (r)) | ((x) >> ((n) - (r))))
#define Pe_ROTL32(x, r) (r == 0 ? x : _Pe_ROTL(x, r, 32))
#define Pe_C1 UINT32_C(0xcc9e2d51)
#define Pe_C2 UINT32_C(0x1b873593)

static Pe_ALWAYS_INLINE
uint32_t PeHash_getK1(const uint8_t* ptr) {
  uint32_t k1 = Pe_LOAD32(ptr);
  k1 *= Pe_C1;
  k1 = Pe_ROTL32(k1, 15);
  k1 *= Pe_C2;
  return k1;
}

static Pe_ALWAYS_INLINE
uint32_t PeHash_updateH1(uint32_t h1, uint32_t k1) {
  h1 ^= k1;
  h1 = Pe_ROTL32(h1, 13);
  h1 = h1 * 5 + UINT32_C(0xe6546b64);
  return h1;
}

static Pe_ALWAYS_INLINE
uint32_t PeHash_fmix32(uint32_t h1) {
  h1 ^= h1 >> 16;
  h1 *= UINT32_C(0x85ebca6b);
  h1 ^= h1 >> 13;
  h1 *= UINT32_C(0xc2b2ae35);
  h1 ^= h1 >> 16;
  return h1;
}

#define Pe_DFL_SEED UINT32_C(0xec4e6c89)
void PeHashContext_init(Pe_HashContext* context) {
  PeHashContext_init2(context, Pe_DFL_SEED);
}

void PeHashContext_init2(Pe_HashContext* context, uint32_t seed) {
  context->h1 = seed;
  context->pos = 0;
  context->len = 0;
}

void PeHashContext_update(Pe_HashContext* context,
                          const void* data, size_t len) {
  if (len <= 0) return;

  const uint8_t* ptr = (const uint8_t*) data;
  uint32_t h1 = context->h1, k1;
  context->len += len;

  if (context->pos > 0) {
    while (context->pos < Pe_STATE_SIZE) {
      context->state[context->pos++] = *ptr++;
      if (--len <= 0)
        return;
    }
    k1 = PeHash_getK1(context->state);
    h1 = PeHash_updateH1(h1, k1);
    context->pos = 0;
  }

  int nblock = len / Pe_STATE_SIZE;
  for (; nblock > 0; --nblock) {
    k1 = PeHash_getK1(ptr);
    h1 = PeHash_updateH1(h1, k1);
    ptr += Pe_STATE_SIZE;
    len -= Pe_STATE_SIZE;
  }

  context->h1 = h1;

  if (len > 0) {
    memcpy(context->state, ptr, len);
    context->pos = len;
  }
}

uint32_t PeHashContext_finish(Pe_HashContext* context) {
  uint32_t h1 = context->h1;

  if (context->pos > 0) {
    memset(context->state + context->pos, 0, Pe_STATE_SIZE - context->pos);
    /* There is different from MurmurHash3 x86_32. */
    uint32_t k1 = PeHash_getK1(context->state);
    k1 *= Pe_C1;
    k1 = Pe_ROTL32(k1, 15);
    k1 *= Pe_C2;
    h1 ^= k1;
  }

  h1 ^= context->len;
  h1 = PeHash_fmix32(h1);

  return h1;
}

uint32_t Pe_getHashValue(const void* data, size_t len) {
  return Pe_getHashValue2(data, len, Pe_DFL_SEED);
}

uint32_t Pe_getHashValue2(const void* data, size_t len, uint32_t seed) {
  Pe_HashContext ctx;
  PeHashContext_init2(&ctx, seed);
  PeHashContext_update(&ctx, data, len);
  return PeHashContext_finish(&ctx);
}

#ifdef PE_HASH_TEST
#include <stdio.h>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("%s \"string to hash\"\n", argv[0]);
    return 0;
  }
  printf("Input: %s\n", argv[1]);

  Pe_HashContext ctx;
  PeHashContext_init2(&ctx, 42);
  for (int i = 1; i < argc; ++i)
    PeHashContext_update(&ctx, argv[i], strlen(argv[i]));
  unsigned int v = PeHashContext_finish(&ctx);
  printf("getHashValue: %#x\n", v);
  return 0;
}
#endif
