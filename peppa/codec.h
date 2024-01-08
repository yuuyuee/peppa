/* Copyright 2023 The PP_ppa Authors. */

#ifndef PEPPA_CODEC_H_
#define PEPPA_CODEC_H_

#include <stdint.h>

#include "peppa/macros.h"

#define PP_LOAD8(p)  *(const uint8_t*) (p)
#define PP_LOAD16(p) *(const uint16_t*) (p)
#define PP_LOAD32(p) *(const uint32_t*) (p)
#define PP_LOAD64(p) *(const uint64_t*) (p)

#define PP_STORE8(p, v)  *(uint8_t*) (p) = (v)
#define PP_STORE16(p, v) *(uint16_t*) (p) = (v)
#define PP_STORE32(p, v) *(uint32_t*) (p) = (v)
#define PP_STORE64(p, v) *(uint64_t*) (p) = (v)

#endif  /* PEPPA_CODEC_H_ */
