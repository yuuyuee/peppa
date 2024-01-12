// Copyright 2023 The PP_ppa Authors.

#ifndef PEPPA_UNALIGNED_ACCESS_H_
#define PEPPA_UNALIGNED_ACCESS_H_

#include <cstdint>
#include <cstring>

#include "peppa/config.h"

namespace peppa {
PP_NAMESPACE_BEGIN

// These function are completely portable of handling unaligned
// load and store, event on RISC architectures.
// http://pzemtsov.github.io/2016/11/06/bug-story-alignment-on-x86.html

inline uint64_t UnalignedLoad64(const void* p) {
  uint64_t value;
  memcpy(&value, p, sizeof(value));
  return value;
}

inline uint32_t UnalignedLoad32(const void* p) {
  uint32_t value;
  memcpy(&value, p, sizeof(value));
  return value;
}

inline uint16_t UnalignedLoad16(const void* p) {
  uint16_t value;
  memcpy(&value, p, sizeof(value));
  return value;
}

inline void UnalignedStore64(void* p, uint64_t value) {
  memcpy(p, &value, sizeof(value));
}

inline void UnalignedStore32(void* p, uint32_t value) {
  memcpy(p, &value, sizeof(value));
}

inline void UnalignedStore16(void* p, uint16_t value) {
  memcpy(p, &value, sizeof(value));
}

PP_NAMESPACE_END
}  // namespace peppa

#endif  // PEPPA_UNALIGNED_ACCESS_H_
