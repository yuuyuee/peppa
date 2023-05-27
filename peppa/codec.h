/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_CODEC_H_
#define PEPPA_CODEC_H_

#include <stdint.h>
#include <stddef.h>

#define PP_INT(bit) int ## bit ## _t
#define PP_UINT(bit) uint ## bit ## _t

#define PP_INT_ACCESS_FN(bit)                                           \
static inline PP_INT(bit) GetInt ## bit(void* buffer) {                 \
  return *(const PP_INT(bit)*) (buffer);                                \
}                                                                       \
                                                                        \
static inline PP_UINT(bit) GetUInt ## bit(void* buffer) {               \
  return *(const PP_UINT(bit)*) (buffer);                               \
}                                                                       \
                                                                        \
static inline void SetInt ## bit(void* buffer, PP_INT(bit) value) {     \
  *(PP_INT(bit)*) (buffer) = value;                                     \
}                                                                       \
                                                                        \
static inline void SetUInt ## bit(void* buffer, PP_UINT(bit) value) {   \
  *(PP_UINT(bit)*) (buffer) = value;                                    \
}

PP_INT_ACCESS_FN(8)
PP_INT_ACCESS_FN(16)
PP_INT_ACCESS_FN(32)
PP_INT_ACCESS_FN(64)

#define PP_ZIGZAG_FN(bit)                                               \
static inline PP_UINT(bit)                                              \
    ZigZagEnode ## bit(PP_INT(bit) value) {                             \
  return (value << 1) ^ (value >> (bit - 1));                           \
}                                                                       \
                                                                        \
static inline PP_INT(bit)                                               \
    ZigZagDecode ## bit(PP_UINT(bit) value) {                           \
  return (value >> 1) ^ -((PP_INT(bit)) (value & 1));                   \
}

PP_ZIGZAG_FN(8)
PP_ZIGZAG_FN(16)
PP_ZIGZAG_FN(32)
PP_ZIGZAG_FN(64)

static inline const void* GetVarint32(const void* buffer, uint32_t* value) {
  const uint8_t* ptr = (const uint8_t*) buffer;
  *value = ptr[0] & 0x7F;
  if (ptr[0] & 0x80) {
    *value |= (ptr[1] & 0x7F) << 7;
    if (ptr[1] & 0x80) {
      *value |= (ptr[2] & 0x7F) << 14;
      if (ptr[2] & 0x80) {
        *value |= (ptr[3] & 0x7F) << 21;
        if (ptr[3] & 0x80) {
          *value |= (ptr[4] & 0x7F) << 28;
          return ptr + 5;
        } else {
          return ptr + 4;
        }
      } else {
        return ptr + 3;
      }
    } else {
      return ptr + 2;
    }
  } else {
    return ptr + 1;
  }
}

static inline void* SetVarint32(void* buffer, uint32_t value) {
  uint8_t* ptr = (uint8_t*) buffer;
  ptr[0] = static_cast<uint8_t>(value | 0x80);
  value >>= 7;
  if (value > 0) {
    ptr[1] = static_cast<uint8_t>(value | 0x80);
    value >>= 7;
    if (value > 0) {
      ptr[2] = static_cast<uint8_t>(value | 0x80);
      value >>= 7;
      if (value > 0) {
        ptr[3] = static_cast<uint8_t>(value | 0x80);
        value >>= 7;
        if (value > 0) {
          ptr[4] = static_cast<uint8_t>(value | 0x7F);
          return ptr + 5;
        } else {
          ptr[3] &= 0x7F;
          return ptr + 4;
        }
      } else {
        ptr[2] &= 0x7F;
        return ptr + 3;
      }
    } else {
      ptr[1] &= 0x7F;
      return ptr + 2;
    }
  } else {
    ptr[0] &= 0x7F;
    return ptr + 1;
  }
}

#endif  /* PEPPA_CODEC_H_ */
