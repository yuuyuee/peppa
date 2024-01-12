// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_ENDIAN_H_
#define PEPPA_ENDIAN_H_

#include <cstdint>
#include <cstdlib>

#include "peppa/config.h"
#include "peppa/unaligned_access.h"

namespace peppa {
PP_NAMESPACE_BEGIN

inline uint64_t ByteSwap64(uint64_t value) {
#if PP_HAVE_BUILTIN(__builtin_bswap64) || defined(__GNUC__)
  return __builtin_bswap64(value);
#elif defined(_MSC_VER)
  return _byteswap_uint64(value);
#else
  return (((value & UINT64_C(0xFF)) << 56) |
          ((value & UINT64_C(0xFF00)) << 40) |
          ((value & UINT64_C(0xFF0000)) << 24) |
          ((value & UINT64_C(0xFF000000)) << 8) |
          ((value & UINT64_C(0xFF00000000)) >> 8) |
          ((value & UINT64_C(0xFF0000000000)) >> 24) |
          ((value & UINT64_C(0xFF000000000000)) >> 40) |
          ((value & UINT64_C(0xFF00000000000000)) >> 56));
#endif
}

inline uint32_t ByteSwap32(uint32_t value) {
#if PP_HAVE_BUILTIN(__builtin_bswap32) || defined(__GNUC__)
  return __builtin_bswap32(value);
#elif defined(_MSC_VER)
  return _byteswap_uint32(value);
#else
  return (((value & UINT32_C(0xFF)) << 24) |
          ((value & UINT32_C(0xFF00)) << 8) |
          ((value & UINT32_C(0xFF0000)) >> 8) |
          ((value & UINT32_C(0xFF000000)) >> 24));
#endif
}

inline uint16_t ByteSwap16(uint16_t value) {
#if PP_HAVE_BUILTIN(__builtin_bswap16) || defined(__GNUC__)
  return __builtin_bswap16(value);
#elif defined(_MSC_VER)
  return _byteswap_ushort(value);
#else
  return (((value & UINT16_C(0xFF)) << 8) |
          ((value & UINT16_C(0xFF00)) >> 8));
#endif
}

inline uint64_t htonll(uint64_t value) {
#if PP_IS_LITTLE_ENDIAN
  return ByteSwap64(value);
#else
  return value;
#endif
}

inline uint32_t htonl(uint32_t value) {
#if PP_IS_LITTLE_ENDIAN
  return ByteSwap32(value);
#else
  return value;
#endif
}

inline uint16_t htons(uint16_t value) {
#if PP_IS_LITTLE_ENDIAN
  return ByteSwap16(value);
#else
  return value;
#endif
}

inline uint64_t ntohll(uint64_t value) {
#if PP_IS_LITTLE_ENDIAN
  return ByteSwap64(value);
#else
  return value;
#endif
}

inline uint64_t ntohll(uint64_t value) { return htonll(value); }
inline uint32_t ntohl(uint32_t value) { return htonl(value); }
inline uint16_t ntohs(uint16_t value) { return htons(value); }

PP_NAMESPACE_END
}  // namespace peppa

#endif  // PEPPA_ENDIAN_H_
