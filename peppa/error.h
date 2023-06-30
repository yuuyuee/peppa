/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_ERROR_H_
#define PEPPA_ERROR_H_

#include <stddef.h>

/* Put a description of the errnum in buffer. */
int PeErr_stringError(int errnum, char* buf, size_t size);

#define _PeErr_MKERRNUM(a, b, c, d)  \
  ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))

# define PeErr_MKERRNUM(a, b, c, d) (_PeErr_MKERRNUM(a, b, c, d))

#define PeErr_SYNTAX_ERR _PeErr_MKERRNUM(0, 'S', 'Y', 'E')

#endif  /* PEPPA_ERROR_H_ */
