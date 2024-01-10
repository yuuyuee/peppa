// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_HASH_H_
#define PEPPA_HASH_H_

#include <cstddef>
#include <stdint.h>

#include "peppa/config.h"

namespace peppa {
PP_NAMESPACE_BEGIN

class HashContext {
 public:
  // Construct an 128-bits hash context.
  HashContext();

  // Construct an 32-bits hash context with a seed.
  HashContext(unsigned seed);

  // Reset the state to start hashing the value of the new stream.
  void Reset();

  // Update hash context with new data.
  void Update(const void* data, size_t len);

  // Finish hashing and returns a 128-bits hash value.
  void Finish(uint64_t out[2]);

 private:
  void UpdateOnce(uint64_t k0, uint64_t k1);

 private:
  uint64_t h0_;
  uint64_t h1_;
  uint8_t state_[16];
  unsigned state_len_;
  unsigned seed_;
  uint64_t len_;
};

// Wrapped functions
void Hash(const void* data, size_t len, uint32_t* value);
void Hash(const void* data, size_t len, uint32_t* value, unsigned seed);
void Hash(const void* data, size_t len, uint64_t* value);
void Hash(const void* data, size_t len, uint64_t* value, unsigned seed);
void Hash(const void* data, size_t len, uint64_t value[2]);
void Hash(const void* data, size_t len, uint64_t value[2], unsigned seed);
#ifdef __SIZEOF_INT128__

#endif

PP_NAMESPACE_END
}  // namespace peppa
#endif  // PEPPA_HASH_H_
