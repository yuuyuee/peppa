// Copyright 2023 The Peppa Authors.

#include <stdio.h>

#include "peppa/format.h"

// This is an simple wrapper for snprintf and vsnprintf.
size_t pp_format(char* buf, size_t size, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int ret = pp_vformat(buf, size, fmt, ap);
  va_end(ap);
  return ret;
}

size_t pp_vformat(char* buf, size_t size, const char* fmt, va_list ap) {
  int ret = vsnprintf(buf, size, fmt, ap);
  if (ret < 0) {
    buf[0] = '\0';
    ret = 0;
  }
  return ret;
}
