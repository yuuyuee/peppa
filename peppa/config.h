// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_CONFIG_H_
#define PEPPA_CONFIG_H_

#include <features.h>

#if defined(__cplusplus)
# include <cstddef>
static_assert(__cplusplus >= 201103L,
              "Peppa require c++ 11 standard or later.");
#endif

#include "peppa/options.h"

#define PP_VERSION 20231227

// For ABI compatibility purposes, namespace annotation are injected
// into the namespace at the beginning/end of each "namespace peppa"
// scope. NOTE: Does not support to write "namespace peppa" in own code
// for any reason, does not support forward declarations of this own
// types, nor does it support specialization of library templates.
// Code that violates the rules may be broken compatibility without warning.
#if defined(__cplusplus) && PP_OPTION_USE_INLINE_NAMESPACE == 1
# define PP_CONCATE_IMPL(x, s) x ## _ ## s
# define PP_CONCATE(x, s) PP_CONCATE_IMPL(x, s)

# ifndef PP_OPTION_INLINE_NAMESPACE_NAME
#   define PP_OPTION_INLINE_NAMESPACE_NAME PP_CONCATE(lts, PP_VERSION)
# endif
# define PP_NAMESPACE_BEGIN inline namespace PP_OPTION_INLINE_NAMESPACE_NAME {
# define PP_NAMESPACE_END }
# define PP_INTERNAL_C_SYMBOL(x) PP_CONCATE(x, PP_OPTION_INLINE_NAMESPACE_NAME)
#else
# define PP_NAMESPACE_BEGIN
# define PP_NAMESPACE_END
# define PP_INTERNAL_C_SYMBOL(x) x
#endif

// This function-like macro takes a single identifier argument
// that is the name of a builtin function. It evalutes to 1 if
// the builtin is supported or 0 if not.
#if defined(__has_builtin)
# define PP_HAVE_BUILTIN(x) __has_builtin(x)
#else
# define PP_HAVE_BUILTIN(x) 0
#endif

// This function-like macro takes a single identifier argument
// that is the name of a feature. It evalutes to 1 if the feature
// is supported or 0 if not.
#if defined(__has_feature)
# define PP_HAVE_FEATURE(x) __has_feature(x)
#else
# define PP_HAVE_FEATURE(x) 0
#endif

#if defined(__has_include)
# define PP_HAVE_INCLUDE(x) __has_include(x)
#else
# define PP_HAVE_INCLUDE(x) 0
#endif

// This function-like macro takes a single identifier argumenet
// that is the name of a GNU-style attribute. It evalutes to 1
// if the attribute is supported or 0 if not.
#if defined(__has_attribute)
# define PP_HAVE_ATTRIBUTE(x) __has_attribute(x)
#else
# define PP_HAVE_ATTRIBUTE(x) 0
#endif

// The function-like macro takes a single argument that is the
// name of a C++11-style attribute. It evalutes to 1 if the
// cpp attribute is supported or 0 if not.
#if defined(__has_cpp_attribute) && defined(__cplusplus)
# define PP_HAVE_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
# define PP_HAVE_CPP_ATTRIBUTE(x) 0
#endif

// Checks platform feature.
#if !defined(__linux__)
# error "Peppa requires linux operator system."
#endif

#if !(defined(__x86_64__) || defined(__aarch64__))
# error "Peppa requires x86 architecture with 64-bit extensions or ARM64."
#endif

// Checks compiler minimum version.
#if defined(__clang__)
# define PP_MIN_CLANG_VERSION(major, minor) \
  ((__clang_major__ << 16) + __clang_minor__ >= ((major) << 16) + (minor))
# if !PP_MIN_CLANG_VERSION(3, 3)
#  error "Peppa require clang version at least 3.2."
# endif
#elif defined(__GNUC__)
# define PP_MIN_GNUC_VERSION(major, minor) \
  ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((major) << 16) + (minor))
# if !PP_MIN_GNUC_VERSION(4, 8)
#  error "Peppa require gcc version at least 4.8."
# endif
#endif

// Checks the endianess of the platform.
#if defined(__BYTE_ORDER__)
# if defined(__ORDER_LITTLE_ENDIAN__) && \
    __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define PP_IS_LITTLE_ENDIAN 1
# elif defined(__ORDER_BIG_ENDIAN__) && \
    __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#  define PP_IS_BIG_ENDIAN 1
# endif
#else
# error "Endian detection is failed."
#endif

// Checks exception handling support.
#if defined(__clang__)
// Use __EXCEPTIONS && __has_feature(cxx_exceptions) to reliably test
// if C++ exceptions are enabled in all versions of Clang.
# if defined(__EXCEPTIONS) && PP_HAVE_FEATURE(cxx_exceptions)
#   define PP_HAVE_EXCEPTION 1
# endif
#elif defined(__GNUC__)
# if PP_MIN_GNUC_VERSION(5, 0)
#   if defined(__cpp_exceptions)
#     define PP_HAVE_EXCEPTION 1
#   endif
# elif defined(__EXCEPTIONS)
#   define PP_HAVE_EXCEPTION 1
# endif
#endif

// Checks runtime type identify(RTTI) support.
#if defined(__GXX_RTTI) || defined(__cpp_rtti)
# define PP_HAVE_RTTI 1
#endif

// Defines the size of the L1 cache for purposes of alignment.
#ifndef PP_CACHELINE_SIZE
# define PP_CACHELINE_SIZE 64
#endif

// Provided the compiler with branch prediction information.
// In general, we will prefer to use actual profile feedback(-fprofile-arcs)
// for this. however, annotating specific branches that are both hot and
// consistently mispredicted is like to yield performance improvements.
#if PP_HAVE_BUILTIN(__builtin_expect) || defined(__GNUC__)
# define PP_EXPECT_TRUE(x) (__builtin_expect((x), 1))
# define PP_EXPECT_FALSE(x) (__builtin_expect((x), 0))
#else
# define PP_EXPECT_TRUE(x) (x)
# define PP_EXPECT_FALSE(x) (x)
#endif

// PP_ALWAYS_INLINE
#if defined(__GNUC__)
# define PP_ALWAYS_INLINE inline __attribute__((always_inline))
#else
# define PP_ALWAYS_INLINE inline
#endif

// PP_NORETURN
#if PP_HAVE_CPP_ATTRIBUTE(noreturn)
# define PP_NORETURN [[noreturn]]
#elif defined(__GNUC__)
# define PP_NORETURN __attribute__((noreturn))
#else
# define PP_NORETURN
#endif

// PP_DEPRECATED
#if PP_HAVE_CPP_ATTRIBUTE(deprecated)
# define PP_DEPRECATED(msg) [[deprecated(msg)]]
#elif defined(__GNUC__)
# define PP_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
# define PP_DEPRECATED(...)
#endif

// PP_NODISCARD
#if PP_HAVE_CPP_ATTRIBUTE(nodiscard)
# define PP_NODISCARD [[nodiscard]]
#elif defined(__GNUC__)
# define PP_NODISCARD __attribute__((warn_unused_result))
#else
# define PP_NODISCARD
#endif

// PP_ALIGNED
#if defined(__GNUC__)
# define PP_ALIGNED(n) __attribute__((aligned(n)))
# define PP_CACHELINE_ALIGNED PP_ALIGNED(PP_CACHELINE_SIZE)
#else
# define PP_ALIGNED(n)
# define PP_CACHELINE_ALIGNED
#endif

#endif  // PEPPA_CONFIG_H_
