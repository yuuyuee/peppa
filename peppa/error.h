/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_ERROR_H_
#define PEPPA_ERROR_H_

#include <stddef.h>
#include <errno.h>

/* Put a description of the errnum in buffer. */
int pp_strerror(int errnum, char* buf, size_t size);

#define PPMKERRNUM_(a, b, c, d)       \
  ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))

#if ENOMEM > 0
# define PPMKERRNUM(a, b, c, d) (-(int) PPMKERRNUM_(a, b, c, d))
#else
# define PPMKERRNUM(a, b, c, d) (PPMKERRNUM_(a, b, c, d))
#endif

#define PP_SYNTAX_ERROR PPMKERRNUM(0, 0, 'S', 'E')

#endif  /* PEPPA_ERROR_H_ */
