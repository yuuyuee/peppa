// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_ATTRIBUTES_H_
#define PEPPA_ATTRIBUTES_H_

#include "peppa/config.h"

// PEPPA_ALWAYS_INLINE
#if defined(__GNUC__)
# define PEPPA_ALWAYS_INLINE inline __attribute__((__always_inline__))
#else
# define PEPPA_ALWAYS_INLINE inline
#endif

// PEPPA_NORETURN
#if defined(__GNUC__)
# define PEPPA_NORETURN __attribute__((noreturn))
#else
# define PEPPA_NORETURN
#endif

// PEPPA_DEPRECATED
#if defined(__GNUC__)
# define PEPPA_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
# define PEPPA_DEPRECATED(...)
#endif

// PEPPA_NODISCARD
#if defined(__GNUC__)
# define PEPPA_NODISCARD __attribute__((warn_unused_result))
#else
# define PEPPA_NODISCARD
#endif

// PEPPA_ALIGNED
#if defined(__GNUC__)
# define PEPPA_ALIGNED(n) __attribute__((aligned(n)))
#else
# define PEPPA_ALIGNED(n)
#endif

#endif  /* PEPPA_ATTRIBUTES_H_ */
