/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_FORMAT_H_
#define PEPPA_FORMAT_H_

#include <stdarg.h>

/* This is an simple wrapper for vsnprintf. */

size_t Pe_fmt(char* buf, size_t size, const char* fmt, ...);
size_t Pe_fmtv(char* buf, size_t size, const char* fmt, va_list ap);

#endif  /* PEPPA_FORMAT_H_ */
