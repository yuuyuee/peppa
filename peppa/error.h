/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_ERROR_H_
#define PEPPA_ERROR_H_

#include <stddef.h>
#include <errno.h>

/* Put a description of the errnum in buffer. */
int PeErr_stringError(int errnum, char* buf, size_t size);

#define _PeErr_MKERRNUM(a, b, c, d)  \
  ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))

#if ENOMEM > 0
# define PeErr_MKERRNUM(a, b, c, d) (-(int) _PeErr_MKERRNUM(a, b, c, d))
#else
# define PeErr_MKERRNUM(a, b, c, d) (_PeErr_MKERRNUM(a, b, c, d))
#endif

#endif  /* PEPPA_ERROR_H_ */
