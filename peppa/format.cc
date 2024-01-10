// Copyright 2023 The Peppa Authors.

#include "peppa/format.h"

#include <cstdio>

namespace peppa {
PP_NAMESPACE_BEGIN

size_t Format(char* buf, size_t size, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  size_t ret = Format(buf, size, fmt, ap);
  va_end(ap);
  return ret;
}

size_t Format(char* buf, size_t size, const char* fmt, va_list ap) {
  int ret = vsnprintf(buf, size, fmt, ap);
  if (ret < 0) {
    buf[0] = '\0';
    ret = 0;
  } else if ((size_t) ret >= size) {
    buf[size - 1] = '\0';
    ret = size - 1;
  }
  return ret;
}

PP_NAMESPACE_END
}  // namespace peppa
