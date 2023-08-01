/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_FORMAT_H_
#define PEPPA_FORMAT_H_

#include <stddef.h>
#include <stdarg.h>

/* This is an simple wrapper for vsnprintf. */

size_t Pe_format(char* buf, size_t size, const char* fmt, ...);
size_t Pe_formatv(char* buf, size_t size, const char* fmt, va_list ap);

#endif  /* PEPPA_FORMAT_H_ */
