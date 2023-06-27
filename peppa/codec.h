/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_CODEC_H_
#define PEPPA_CODEC_H_

#include <stdint.h>
#include <stddef.h>

#define _Pe_INT(bit) int ## bit ## _t
#define _Pe_UINT(bit) uint ## bit ## _t

#define PeCodec_LOAD8(p) (*(const uint8_t*) (p))
#define PeCodec_LOAD16(p) (*(const uint16_t*) (p))
#define PeCodec_LOAD32(p) (*(const uint32_t*) (p))
#define PeCodec_LOAD64(p) (*(const uint64_t*) (p))

#define PeCodec_STORE8(p, v) (*(uint8_t*) (p) = v)
#define PeCodec_STORE16(p, v) (*(uint16_t*) (p) = v)
#define PeCodec_STORE32(p, v) (*(uint32_t*) (p) = v)
#define PeCodec_STORE64(p, v) (*(uint64_t*) (p) = v)

#endif  /* PEPPA_CODEC_H_ */
