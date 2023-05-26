// Copyright 2023 The Peppa Authors.

#ifndef PEPPA_FORMAT_H_
#define PEPPA_FORMAT_H_

#include <stdarg.h>

#include "peppa/macros.h"

// This is an simple wrapper for snprintf and vsnprintf.
size_t pp_format(char* buf, size_t size, const char* fmt, ...)
    PP_ATTR_PRINTF(3, 4);

size_t pp_vformat(char* buf, size_t size, const char* fmt, va_list ap);

#endif  // PEPPA_FORMAT_H_
