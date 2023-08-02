/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_ERROR_H_
#define PEPPA_ERROR_H_

#include <stddef.h>

/* Put a description of the errnum in buffer. */
int Pe_stringError(int errnum, char* buf, size_t size);

void _Pe_Error(int errnum, const char* fmt, ...);

#define _PeErr_MKERRNUM(a, b, c, d)  \
  (((unsigned)(a) << 24) | ((b) << 16) | ((c) << 8) | (d))

# define PeErr_MKERRNUM(a, b, c, d) (_PeErr_MKERRNUM(a, b, c, d))

#define PeErr_SYNTAX_ERR _PeErr_MKERRNUM(0, 'S', 'Y', 'E')

#endif  /* PEPPA_ERROR_H_ */
