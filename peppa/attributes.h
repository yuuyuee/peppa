/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_ATTRIBUTES_H_
#define PEPPA_ATTRIBUTES_H_

/* PeAttr_HAS_ATTRIBUTE() */
#if defined(__has_attribute)
# define PeAttr_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
# define PeAttr_HAS_ATTRIBUTE(x) (0)
#endif

/* PeAttr_HAS_BUILTIN() */
#if defined(__has_builtin)
# define PeAttr_HAS_BUILTIN(x) __has_builtin(x)
#else
# define PeAttr_HAS_BUILTIN(x) 0
#endif

/* PeAttr_ALWAYS_INLINE() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PeAttr_HAS_ATTRIBUTE(always_inline)
# define PeAttr_ALWAYS_INLINE inline __attribute__((always_inline))
#else
# define PeAttr_ALWAYS_INLINE inline
#endif

/* PeAttr_NORETURN() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PeAttr_HAS_ATTRIBUTE(noreturn)
# define PeAttr_NORETURN __attribute__((noreturn))
#else
# define PeAttr_NORETURN
#endif

/* PeAttr_DEPRECATED() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PeAttr_HAS_ATTRIBUTE(deprecated)
# define PeAttr_DEPRECATED(...) __attribute__((deprecated(__VA_ARGS__)))
#else
# define PeAttr_DEPRECATED(...)
#endif

/* PeAttr_MAYBE_UNUSED() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PeAttr_HAS_ATTRIBUTE(unused)
# define PeAttr_MAYBE_UNUSED __attribute__((unused))
#else
# define PeAttr_MAYBE_UNUSED
#endif

/* PeAttr_NODISCARD() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PeAttr_HAS_ATTRIBUTE(warn_unused_result)
# define PeAttr_NODISCARD __attribute__((warn_unused_result))
#else
# define PeAttr_NODISCARD
#endif

/* PeAttr_ALIGNED() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PeAttr_HAS_ATTRIBUTE(aligned)
# define PeAttr_ALIGNED(n) __attribute__((aligned(n)))
#else
# define PeAttr_ALIGNED(n)
#endif

/* PeAttr_PACKED() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PeAttr_HAS_ATTRIBUTE(packed)
# define PeAttr_PACKED __attribute__((packed))
#else
# define PeAttr_PACKED
#endif

/* PeAttr_LIKELY(), PeAttr_UNLIKELY() */
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PeAttr_HAS_BUILTIN(__builtin_expect)
# define PPLIKELY(x) __builtin_expect(false || (x), true)
# define PPUNLIKELY(x) __builtin_expect(false || (x), false)
#else
# define PPLIKELY(x) (x)
# define PPUNLIKELY(x) (x)
#endif

#endif  /* PEPPA_ATTRIBUTES_H_ */
