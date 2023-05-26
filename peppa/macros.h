// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_MACROS_H_
#define PEPPA_MACROS_H_

#include <stddef.h>  // size_t

#if defined(__cplusplus)
# define PP_ARRAYSIZE(a) (sizeof(peppa::macros_internal::ArraySizeHelper(a)))

namespace peppa {
namespace macros_internal {
template <typename T, size_t N, typename R = char (&)[N]>
R ArraySizeHelper(const T (&)[N]);
}  // namespace macros_internal
}  // namespace peppa
#else
# define PP_ARRAYSIZE(a) (sizeof(a) / sizeof(*(a)))
#endif

#define PPMAX(a, b) ((a) > (b) ? (a) : (b))
#define PPMAX3(a, b, c) PPMAX(PPMAX(a, b), c)
#define PPMIN(a, b) ((a) < (b) ? (a) : (b))
#define PPMIN3(a, b, c) PPMIN(PPMIN(a, b), c)

#define PPSWAP(type, a, b) do {   \
  type tmp = b;                   \
  b = a;                          \
  a = tmp;                        \
} while (0)

#define PPSTR(s) PP_STR_(s)
#define PPSTR_(s) #s

#define PPJOIN(a, b) PP_JOIN_(a, b)
#define PPJOIN_(a, b) a ## b

#define PPALIGIN(n, a) (((n) + (a) - 1) & ~((a) - 1))
#define PPCOMPARE(a, b) (((a) > (b)) - ((a) < (b)))

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
# define PP_HAS_BE 1
#else
# define PP_HAS_LE 1
#endif

// PP_HAS_ATTRIBUTE()
#if defined(__has_attribute)
# define PP_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
# define PP_HAS_ATTRIBUTE(x) (0)
#endif

// PP_HAS_CPP_ATTRIBUTE()
#if defined(__has_cpp_attribute)
# define PP_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
# define PP_HAS_CPP_ATTRIBUTE(x) (0)
#endif

// PP_ATTR_ALWAYS_INLINE()
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PP_HAS_ATTRIBUTE(always_inline)
# define PP_ATTR_ALWAYS_INLINE inline __attribute__((always_inline))
#else
# define PP_ATTR_ALWAYS_INLINE
#endif

// PP_ATTR_NORETURN()
#if defined(__cplusplus) && __cplusplus >= 201103L
# define PP_ATTR_NORETURN [[noreturn]]
#elif (defined(__GNUC__) && !defined(__clang__)) || \
    PP_HAS_ATTRIBUTE(noreturn)
# define PP_ATTR_NORETURN __attribute__((noreturn))
#else
# define PP_ATTR_NORETURN
#endif

// PP_ATTR_DEPRECATED()
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PP_HAS_ATTRIBUTE(deprecated)
# define PP_ATTR_DEPRECATED(...) __attribute__((deprecated(__VA_ARGS__)))
#else
# define PP_ATTR_DEPRECATED(...)
#endif

// PP_ATTR_MAYBE_UNUSED()
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PP_HAS_ATTRIBUTE(unused)
# define PP_ATTR_MAYBE_UNUSED __attribute__((unused))
#else
# define PP_ATTR_MAYBE_UNUSED
#endif

// PP_ATTR_NODISCARD()
#if defined(__cplusplus) && __cplusplus >= 201103L
# define PP_ATTR_NODISCARD [[nodiscard]]
#elif (defined(__GNUC__) && !defined(__clang__)) || \
    PP_HAS_ATTRIBUTE(warn_unused_result)
# define PP_ATTR_NODISCARD __attribute__((warn_unused_result))
#else
# define PP_ATTR_NODISCARD
#endif

// PP_ATTR_ALIGNED()
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PP_HAS_ATTRIBUTE(aligned)
# define PP_ATTR_ALIGNED(n) __attribute__((aligned(n)))
#else
# define PP_ATTR_ALIGNED(n)
#endif

// PP_ATTR_PACKED()
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PP_HAS_ATTRIBUTE(packed)
# define PP_ATTR_PACKED __attribute__((packed))
#else
# define PP_ATTR_PACKED
#endif

// PP_ATTR_PRINTF()
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PP_HAS_ATTRIBUTE(__format__)
# define PP_ATTR_PRINTF(index, first) \
    __attribute__((__format__(__printf__, index, first)))
#else
# define PP_ATTR_PRINTF(index, first)
#endif

// PP_HAS_BUILTIN()
#if defined(__has_builtin)
# define PP_HAS_BUILTIN(x) __has_builtin(x)
#else
# define PP_HAS_BUILTIN(x) 0
#endif

// PP_LIKELY(), PP_UNLIKELY()
#if (defined(__GNUC__) && !defined(__clang__)) || \
    PP_HAS_BUILTIN(__builtin_expect)
# define PP_LIKELY(x) __builtin_expect(false || (x), true)
# define PP_UNLIKELY(x) __builtin_expect(false || (x), false)
#else
# define PP_LIKELY(x) (x)
# define PP_UNLIKELY(x) (x)
#endif

#if defined(__cplusplus)

// IGNORE_UNUNSED()
#define IGNORE_UNUESD(...) oak::macros_internal::IgnoreUnused(__VA_ARGS__)

namespace peppa {
namespace macros_internal {
template <typename... Args>
constexpr int IgnoreUnused(Args&&...) { return 0; }

template <typename...>
constexpr int IgnoreUnused() { return 0; }
}  // namespace macros_internal
}  // namespace peppa

// PP_ASSERT()
// This is an runtime assert and that can be used portably within
// constexpr functions.
#if defined(NDEBUG)
# define PP_ASSERT(cond)  \
    (false ? static_cast<void>(cond) : static_cast<void>(0))
#else
# include <assert.h>
# define PP_ASSERT(cond)                    \
    (PP_LIKELY(cond)                        \
        ? static_cast<void>(0)              \
        : [] { assert(false && #cond); }())
#endif

#define PP_DISALLOW_COPY_AND_ASSIGN(type)     \
    type(type const&) = delete;               \
    type& operator=(type const&) = delete

#endif

#endif  // PEPPA_MACROS_H_
