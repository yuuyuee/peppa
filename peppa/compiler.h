/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_COMPILER_H_
#define PEPPA_COMPILER_H_

#if !defined(__linux__) && !defined(linux) && !defined(__linux)
# error "Peppa requires linux operator system."
#endif

#if !defined(__x86_64__) && !defined(__x86_64)
# error "Peppa requires x86 architecture with 64-bit extensions."
#endif

#if defined(__GNUC__) && !defined(__clang__)
# define PP_CC_PREREQ(major, minor) \
  ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((major) << 16) + (minor))

# if !PP_CC_PREREQ(4, 8)
#  error "Peppa require gcc version at least 4.8."
# endif
#elif defined(__clang__)
# define PP_CC_PREREQ(major, minor) \
  ((__clang_major__ << 16) + __clang_minor__ >= ((major) << 16) + (minor))

# if !PP_CC_PREREQ(3, 2)
#  error "Peppa require clang version at least 3.2."
# endif
#else  // !defined(__GNUC__) && !defined(__clang__)
# error "Peppa requires GCC or clang compiler."
#endif

#include <features.h>

# define PP_GLIBC_PREREQ(major, minor) \
  ((__GLIBC__ << 16) + __GLIBC_MINOR__ >= ((major) << 16) + (minor))

#if !defined(__GLIBC__) || !PP_GLIBC_PREREQ(2, 17)
# error "Peppa require GNU C standard library version at least 2.17."
#endif

#if defined(__cplusplus)
# if __cplusplus < 201103L
#  error "Peppa require c++ 11 standard."
# endif

#include <cstddef>

# if !defined(__GLIBCXX__) || __GLIBCXX__ < 20150623
#  error "Peppa require GNU C++ standard library at least 20150623."
# endif
#endif

#endif  // PEPPA_COMPILER_H_
