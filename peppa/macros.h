// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_MACROS_H_
#define PEPPA_MACROS_H_

#include "peppa/config.h"

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

namespace peppa {
PP_NAMESPACE_BEGIN

namespace macros_internal {
template <typename T, size_t N, typename R = char (&)[N]>
R ArraySizeHelper(const T (&)[N]);

template <typename... Args>
PP_ALWAYS_INLINE constexpr void IgnoreUnused(Args const& ...) {}

template <typename... Args>
PP_ALWAYS_INLINE constexpr void IgnoreUnused() {}

}  // namespace macros_internal
PP_NAMESPACE_END
}  // namespace peppa

#define PP_ARRAYSIZE(a) sizeof(::peppa::macros_internal::ArraySizeHelper(a))
#define PP_IGNORE_UNUSED(...) ::peppa::macros_internal::IgnoreUnused(##__VA_ARGS__)

#define OAK_DISALLOW_COPY_AND_ASSIGN(type)    \
  type(type const&) = delete;                 \
  type& operator=(type const&) = delete

#endif  // PEPPA_MACROS_H_
