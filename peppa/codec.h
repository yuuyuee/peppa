// Copyright 2023 The PP_ppa Authors.

#ifndef PEPPA_CODEC_H_
#define PEPPA_CODEC_H_

#include <cstdint>
#include <type_traits>

#include "peppa/config.h"

namespace peppa {
PP_NAMESPACE_BEGIN

template <typename Tp, bool = std::is_integral<Tp>::value>
static inline Tp Decode(const void* p) {
  return *reinterpret_cast<const Tp*>(p);
}

template <typename Tp, bool = std::is_integral<Tp>::value>
static inline void Encode(void* p, Tp value) {
  return *reinterpret_cast<Tp*>(p) = value;
}

PP_NAMESPACE_END
}  // namespace peppa

#endif  // PEPPA_CODEC_H_
