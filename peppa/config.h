// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_CONFIG_H_
#define PEPPA_CONFIG_H_

#include <features.h>
#ifdef __cplusplus
# include <cstddef>
static_assert(__cplusplus >= 201103L,
              "Peppa require c++ 11 standard or later.");
#endif

#include "peppa/options.h"

// This function-like macro takes a single identifier argument
// that is the name of a builtin function. It evalutes to 1 if
// the builtin is supported or 0 if not.
#if defined(__has_builtin)
# define PEPPA_HAVE_BUILTIN(x) __has_builtin(x)
#else
# define PEPPA_HAVE_BUILTIN(x) 0
#endif

// This function-like macro takes a single identifier argument
// that is the name of a feature. It evalutes to 1 if the feature
// is supported or 0 if not.
#if defined(__has_feature)
# define PEPPA_HAVE_FEATURE(x) __has_feature(x)
#else
# define PEPPA_HAVE_FEATURE(x) 0
#endif

#if defined(__has_include)
# define PEPPA_HAVE_INCLUDE(x) __has_include(x)
#else
# define PEPPA_HAVE_INCLUDE(x) 0
#endif

// This function-like macro takes a single identifier argumenet
// that is the name of a GNU-style attribute. It evalutes to 1
// if the attribute is supported or 0 if not.
#if defined(__has_attribute)
# define PEPPA_HAVE_ATTRIBUTE(x) __has_attribute(x)
#else
# define PEPPA_HAVE_ATTRIBUTE(x) 0
#endif

// The function-like macro takes a single argument that is the
// name of a C++11-style attribute. It evalutes to 1 if the
// cpp attribute is supported or 0 if not.
#if defined(__has_cpp_attribute) && defined(__cplusplus)
# define PEPPA_HAVE_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
# define PEPPA_HAVE_CPP_ATTRIBUTE(x) 0
#endif

// Checks platform feature.
#if !(defined(__linux__))
# error "Peppa requires linux operator system."
#endif

#if !(defined(__x86_64__) || defined(__aarch64__))
# error "Peppa requires x86 architecture with 64-bit extensions or ARM64."
#endif

// Checks compiler minimum version.
#if defined(__clang__) && defined(__clang_major__) && defined(__clang_minor__)
# define PEPPA_MIN_CLANG_VERSION(major, minor) \
  ((__clang_major__ << 16) + __clang_minor__ >= ((major) << 16) + (minor))
# if !PEPPA_MIN_CLANG_VERSION(3, 3)
#  error "Peppa require clang version at least 3.2."
# endif
#elif defined(__GNUC__) && defined(__GNUC_MINOR__)
# define PEPPA_MIN_GNUC_VERSION(major, minor) \
  ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((major) << 16) + (minor))
# if !PEPPA_MIN_GNUC_VERSION(4, 8)
#  error "Peppa require gcc version at least 4.8."
# endif
#else  // !defined(__GNUC__) && !defined(__clang__)
# error "Peppa requires GCC 4.8+ or clang 3.3+ compiler."
#endif

// Checks the endianess of the platform.
#if defined(__BYTE_ORDER__)
# if defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define PEPPA_IS_LITTLE_ENDIAN 1
# elif defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#  define PEPPA_IS_BIG_ENDIAN 1
# endif
#else
# error "Endian detection is failed."
#endif

// Checks exception handling support.
#if defined(__clang__)
// Use __EXCEPTIONS && __has_feature(cxx_exceptions) to reliably test
// if C++ exceptions are enabled in all versions of Clang.
# if defined(__EXCEPTIONS) && PEPPA_HAVE_FEATURE(cxx_exceptions)
#   define PEPPA_HAVE_EXCEPTION 1
# endif
#elif defined(__GNUC__)
# if PEPPA_MIN_GNUC_VERSION(5, 0)
#   if defined(__cpp_exceptions)
#     define PEPPA_HAVE_EXCEPTION 1
#   endif
# elif defined(__EXCEPTIONS)
#   define PEPPA_HAVE_EXCEPTION 1
# endif
#endif

// Checks runtime type identify(RTTI) support.
#if defined(__GNUC__) && defined(__GXX_RTTI)
# define PEPPA_HAVE_RTTI 1
#endif

// Defines the size of the L1 cache for purposes of alignment.

#endif  /* PEPPA_CONFIG_H_ */
