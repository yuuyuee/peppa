/* Copyright 2023 The Peppa Authors. */

#include "peppa/format.h"

#include <stdio.h>

size_t Pe_format(char* buf, size_t size, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  size_t ret = Pe_formatv(buf, size, fmt, ap);
  va_end(ap);
  return ret;
}

size_t Pe_formatv(char* buf, size_t size, const char* fmt, va_list ap) {
  int ret = vsnprintf(buf, size, fmt, ap);
  if (ret < 0) {
    buf[0] = '\0';
    ret = 0;
  } else if (ret >= size) {
    buf[size - 1] = '\0';
    ret = size - 1;
  }
  return ret;
}
