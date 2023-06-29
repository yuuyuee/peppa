/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_CODEC_H_
#define PEPPA_CODEC_H_

#include <stdint.h>

#include "peppa/macros.h"

#define Pe_LOAD8(p)  *Pe_CAST(const uint8_t*, p)
#define Pe_LOAD16(p) *Pe_CAST(const uint16_t*, p)
#define Pe_LOAD32(p) *Pe_CAST(const uint32_t*, p)
#define Pe_LOAD64(p) *Pe_CAST(const uint64_t*, p)

#define Pe_STORE8(p, v)  *Pe_CAST(uint8_t*, p) = v
#define Pe_STORE16(p, v) *Pe_CAST(uint16_t*, p) = v
#define Pe_STORE32(p, v) *Pe_CAST(uint32_t*, p) = v
#define Pe_STORE64(p, v) *Pe_CAST(uint64_t*, p) = v

#endif  /* PEPPA_CODEC_H_ */
