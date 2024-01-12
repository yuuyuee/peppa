/* Copyright 2023 The Peppa Authors. */

#include "peppa/hash.h"

#include <string.h>

#include "peppa/macros.h"
#include "peppa/error.h"
#include "peppa/unaligned_access.h"

namespace peppa {
PP_NAMESPACE_BEGIN

#define PP_DFT_SEED UINT32_C(0xc2b2ae35)
#define C0 UINT64_C(0x87c37b91114253d5)
#define C1 UINT64_C(0x4cf5ad432745937f)
#define ROTL64(x, r) (((x) << (r)) | ((x) >> (64 - (r))))
#define FMIX64(k)                     \
  k ^= k >> 33;                       \
  k *= UINT64_C(0xff51afd7ed558ccd);  \
  k ^= k >> 33;                       \
  k *= UINT64_C(0xc4ceb9fe1a85ec53);  \
  k ^= k >> 33

HashContext::HashContext(): HashContext(PP_DFT_SEED) {}
HashContext::HashContext(unsigned seed)
    : h0_(seed), h1_(seed), state_(), state_len_(0), seed_(seed), len_(0) {}

void HashContext::Reset() {
  h0_ = seed_;
  h1_ = seed_;
  state_len_ = 0;
}

void HashContext::UpdateOnce(uint64_t k0, uint64_t k1) {
  k0 *= C0;
  k0 = ROTL64(k0, 31);
  k0 *= C1;

  h0_ ^= k0;
  h0_ = ROTL64(h0_, 27);
  h0_ += h1_;
  h0_ = h0_ * 5 + UINT32_C(0x52dce729);

  k1 *= C1;
  k1 = ROTL64(k1, 33);
  k1 *= C0;

  h1_ ^= k1;
  h1_ = ROTL64(h1_, 31);
  h1_ += h0_;
  h1_ = h1_ * 5 + UINT32_C(0x38495ab5);
}

void HashContext::Update(const void* data, size_t len) {
  PP_ASSERT(data != nullptr);
  if (len == 0) return;

  const uint8_t* ptr = static_cast<const uint8_t*>(data);
  if (state_len_ > 0) {
    while (state_len_< sizeof(state_)) {
      state_[state_len_++] = *ptr++;
      if (--len == 0)
        return;
    }

    uint64_t k0 = UnalignedLoad64(state_);
    uint64_t k1 = UnalignedLoad64(state_ + 8);
    UpdateOnce(k0, k1);
    state_len_ = 0;
  }

  int num_block = len / sizeof(state_);
  for (int i = 0; i < num_block; ++i) {
    uint64_t k0 = UnalignedLoad64(ptr);
    uint64_t k1 = UnalignedLoad64(ptr + sizeof(state_) / 2);
    UpdateOnce(k0, k1);
    ptr += sizeof(state_);
    len -= sizeof(state_);
    len_ += sizeof(state_);
  }

  if (len > 0) {
    memcpy(state_, ptr, len);
    state_len_ = len;
  }
}

void HashContext::Finish(uint64_t out[2]) {
  if (state_len_ > 0) {
    PP_ASSERT(state_len_ < sizeof(state_));
    uint64_t k0 = 0;
    uint64_t k1 = 0;
    switch (state_len_) {
      case 15: k1 ^= static_cast<uint64_t>(state_[14]) << 48;
      case 14: k1 ^= static_cast<uint64_t>(state_[13]) << 40;
      case 13: k1 ^= static_cast<uint64_t>(state_[12]) << 32;
      case 12: k1 ^= static_cast<uint64_t>(state_[11]) << 24;
      case 11: k1 ^= static_cast<uint64_t>(state_[10]) << 16;
      case 10: k1 ^= static_cast<uint64_t>(state_[ 9]) << 8;
      case  9: k1 ^= static_cast<uint64_t>(state_[ 8]) << 0;
        k1 *= C1;
        k1  = ROTL64(k1,33);
        k1 *= C0;
        h1_ ^= k1;

      case  8: k0 ^= static_cast<uint64_t>(state_[ 7]) << 56;
      case  7: k0 ^= static_cast<uint64_t>(state_[ 6]) << 48;
      case  6: k0 ^= static_cast<uint64_t>(state_[ 5]) << 40;
      case  5: k0 ^= static_cast<uint64_t>(state_[ 4]) << 32;
      case  4: k0 ^= static_cast<uint64_t>(state_[ 3]) << 24;
      case  3: k0 ^= static_cast<uint64_t>(state_[ 2]) << 16;
      case  2: k0 ^= static_cast<uint64_t>(state_[ 1]) << 8;
      case  1: k0 ^= static_cast<uint64_t>(state_[ 0]) << 0;
        k0 *= C0;
        k0  = ROTL64(k0,31);
        k0 *= C1;
        h0_ ^= k0;
    }
    len_ += state_len_;
  }

  h0_ ^= len_;
  h1_ ^= len_;
  h0_ += h1_;
  h1_ += h0_;
  FMIX64(h0_);
  FMIX64(h1_);
  h0_ += h1_;
  h1_ += h0_;
  out[0] = h0_;
  out[1] = h1_;
}

void Hash(const void* data, size_t len, uint32_t* value) {

}

void Hash(const void* data, size_t len, uint32_t* value, unsigned seed) {

}

void Hash(const void* data, size_t len, uint64_t* value) {

}

void Hash(const void* data, size_t len, uint64_t* value, unsigned seed) {

}

void Hash(const void* data, size_t len, uint64_t value[2]) {
  HashContext context;
  context.Update(data, len);
  context.Finish(value);
}

void Hash(const void* data, size_t len, uint64_t value[2], unsigned seed) {
  HashContext context(seed);
  context.Update(data, len);
  context.Finish(value);
}

PP_NAMESPACE_END
}  // namespace peppa

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
