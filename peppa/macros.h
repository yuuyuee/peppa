/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_MACROS_H_
#define PEPPA_MACROS_H_

#include <stddef.h>

#include "peppa/config.h"

#define PP_MAX(a, b) ((a) > (b) ? (a) : (b))
#define PP_MAX3(a, b, c) PP_MAX(PP_MAX(a, b), c)
#define PP_MIN(a, b) ((a) < (b) ? (a) : (b))
#define PP_MIN3(a, b, c) PP_MIN(PP_MIN(a, b), c)

#define PP_SWAP(type, a, b) do {  \
  type tmp = (b);                 \
  (b) = (a);                      \
  (a) = tmp;                      \
} while (0)

#ifndef PP_STRINGFY
# define PP_STRINGFY_IMPL(s) #s
# define PP_STRINGFY(s) PP_STRINGFY_IMPL(s)
#endif

#ifndef PP_JOIN
# define PP_JOIN_IMPL(a, b) a ## b
# define PP_JOIN(a, b) PP_JOIN_IMPL(a, b)
#endif

#define PP_ALIGIN(n, a) (((n) + (a) - 1) & ~((a) - 1))

#ifdef offsetof
# define PP_OFFSETOF(type, member) offsetof(type, member)
#else
# define PP_OFFSETOF(type, member) ((size_t) &((type*) 0)->member)
#endif

#define PP_CONTAINER_OF(ptr, type, member) \
  ((type *) (((char *) ptr) - PP_OFFSETOF(type, member)))

#ifdef __cplusplus
namespace peppa {
namespace macros_internal {
template <typename T, size_t N, typename R = char (&)[N]>
R ArraySizeHelper(const T (&)[N]);

template <typename... Args>
PP_ALWAYS_INLINE constexpr void IgnoreUnused(Args const& ...) {}

template <typename... Args>
PP_ALWAYS_INLINE constexpr void IgnoreUnused() {}
}  // namespace macros_internal
}  // namespace peppa

# define PP_ARRAYSIZE(a) sizeof(::peppa::macros_internal::ArraySizeHelper(a))
# define PP_IGNORE_UNUSED(...) ::peppa::macros_internal::IgnoreUnused(##__VA_ARGS__)
# define OAK_DISALLOW_COPY_AND_ASSIGN(type)      \
    type(type const&) = delete;                 \
    type& operator=(type const&) = delete
#else
# define PP_ARRAYSIZE(a) (sizeof(a) / sizeof(*(a)))
#endif

#endif  /* PEPPA_MACROS_H_ */
