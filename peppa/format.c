/* Copyright 2023 The Peppa Authors. */

#include "peppa/format.h"
#include <stdio.h>

size_t Pe_fmt(char* buf, size_t size, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int ret = Pe_fmtv(buf, size, fmt, ap);
  va_end(ap);
  return ret;
}

size_t Pe_fmtv(char* buf, size_t size, const char* fmt, va_list ap) {
  int ret = vsnprintf(buf, size, fmt, ap);
  if (ret < 0) {
    buf[0] = '\0';
    ret = 0;
  }
  return ret;
}
