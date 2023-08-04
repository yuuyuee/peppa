/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_ATTRIBUTES_H_
#define PEPPA_ATTRIBUTES_H_

/* Pe_HAS_ATTRIBUTE() */
#if defined(__has_attribute)
# define Pe_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
# define Pe_HAS_ATTRIBUTE(x) (0)
#endif

/* Pe_HAS_BUILTIN() */
#if defined(__has_builtin)
# define Pe_HAS_BUILTIN(x) __has_builtin(x)
#else
# define Pe_HAS_BUILTIN(x) 0
#endif

/* Pe_ALWAYS_INLINE() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    Pe_HAS_ATTRIBUTE(always_inline)
# define Pe_ALWAYS_INLINE inline __attribute__((always_inline))
#else
# define Pe_ALWAYS_INLINE inline
#endif

/* Pe_NORETURN() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    Pe_HAS_ATTRIBUTE(noreturn)
# define Pe_NORETURN __attribute__((noreturn))
#else
# define Pe_NORETURN
#endif

/* Pe_DEPRECATED() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    Pe_HAS_ATTRIBUTE(deprecated)
# define Pe_DEPRECATED(...) __attribute__((deprecated(__VA_ARGS__)))
#else
# define Pe_DEPRECATED(...)
#endif

/* Pe_MAYBE_UNUSED() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    Pe_HAS_ATTRIBUTE(unused)
# define Pe_MAYBE_UNUSED __attribute__((unused))
#else
# define Pe_MAYBE_UNUSED
#endif

/* Pe_NODISCARD() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    Pe_HAS_ATTRIBUTE(warn_unused_result)
# define Pe_NODISCARD __attribute__((warn_unused_result))
#else
# define Pe_NODISCARD
#endif

/* Pe_ALIGNED() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    Pe_HAS_ATTRIBUTE(aligned)
# define Pe_ALIGNED(n) __attribute__((aligned(n)))
#else
# define Pe_ALIGNED(n)
#endif

/* Pe_PACKED() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    Pe_HAS_ATTRIBUTE(packed)
# define Pe_PACKED __attribute__((packed))
#else
# define Pe_PACKED
#endif

/* Pe_LIKELY(), Pe_UNLIKELY() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    Pe_HAS_BUILTIN(__builtin_expect)
# define Pe_LIKELY(x) __builtin_expect(!!(x), 1)
# define Pe_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
# define Pe_LIKELY(x) (x)
# define Pe_UNLIKELY(x) (x)
#endif

#endif  /* PEPPA_ATTRIBUTES_H_ */
