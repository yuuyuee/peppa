/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_ERROR_H_
#define PEPPA_ERROR_H_

#include <stddef.h>
#include <stdlib.h>

#include "peppa/attributes.h"

#define _Pe_MKERRNUM(a, b, c, d)  \
  (((unsigned)(a) << 24) | ((b) << 16) | ((c) << 8) | (d))

# define Pe_MKERRNUM(a, b, c, d) _Pe_MKERRNUM(a, b, c, d)

#define PeErr_SYNTAX_ERROR Pe_MKERRNUM(0, 'S', 'Y', 'E')

/* Put a description of the errnum in "buf". */
const char* Pe_strError(int errnum, char* buf, size_t size);

#ifndef NDEBUG
# define PE_CHECK(cond, fmt, ...) do {                            \
  if (Pe_UNLIKELY(!(cond))) {                                     \
    _Pe_error(__FILE__, __LINE__, __func__, fmt, ## __VA_ARGS__); \
    abort();                                                      \
  }                                                               \
} while (0)

# define PE_CHECK2(cond) PE_CHECK(cond, #cond)

void _Pe_error(const char* file, int line, const char* fun,
               const char* fmt, ...);
#else
# define PE_CHECK(cond, fmt, ...)
# define PE_CHECK2(cond)
#endif

#endif  /* PEPPA_ERROR_H_ */
