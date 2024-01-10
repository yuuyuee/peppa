// Copyright 2023 The Peppa Authors.

#include "peppa/error.h"

// XSI-compliant version of strerror_r
#ifdef _GNU_SOURCE
# undef _GNU_SOURCE
# undef __USE_GNU
#endif

#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 600
#endif

#include <cstring>
#include <cstdarg>
#include <cerrno>

#include "peppa/macros.h"

namespace peppa {
PP_NAMESPACE_BEGIN

namespace {
static const struct ErrorEntry {
  int num;
  const char* str;
} error_entries[] = {
  { PP_SYNTAX_ERROR, "Syntax error" },
};

unsigned CharLength(int value) {
  unsigned n = 1;
  const unsigned base = 10;
  const unsigned b2 = base * base;
  const unsigned b3 = b2 * base;
  const unsigned b4 = b3 * base;

  for (;;) {
    if (value < base) return n;
    if (value < b2) return n + 1;
    if (value < b3) return n + 2;
    if (value < b4) return n + 3;
    value /= b4;
    n += 4;
  }
}

void ToString(int value, char* buf) {
  bool neg = value < 0;
  if (neg) *buf++ = '-';
  unsigned u_value = neg ? static_cast<unsigned>(~value) + 1u : value;
  static const char digits[] =
    "0001020304050607080910111213141516171819"
    "2021222324252627282930313233343536373839"
    "4041424344454647484950515253545556575859"
    "6061626364656667686970717273747576777879"
    "8081828384858687888990919293949596979899";
  const unsigned len = CharLength(u_value);
  unsigned pos = len - 1;
  while (u_value > 100) {
    auto const num = u_value % 100 * 2;
    u_value /= 100;
    buf[pos--] = digits[num + 1];
    buf[pos--] = digits[num];
  }

  if (u_value >= 10) {
    auto const num = u_value * 2;
    buf[1] = digits[num + 1];
    buf[0] = digits[num];
  } else {
    buf[0] = '0' + u_value;
  }
}
}  // anonymous namespace

const char* StrError(int errnum, char* buf, size_t size) {
  const struct ErrorEntry* entry = NULL;
  for (auto const& err : error_entries) {
    if (err.num == errnum) {
      entry = &err;
      break;
    }
  }
  if (entry) {
    size_t len = strlen(entry->str);
    if (len >= size)
      len = size - 1;
    strncpy(buf, entry->str, len);
    buf[len] = '\0';
  } else {
    int save_errno = errno;
    int ret = strerror_r(errnum, buf, size);
    if (ret != 0) {
      PP_ASSERT(errno == EINVAL);
      PP_ASSERT(size >= 64);
      static const char* const unk = "Unkown error ";
      buf = strncpy(buf, unk, strlen(unk));
      ToString(buf, errnum);
    }
    errno = save_errno;
  }
  return buf;
}

PP_NAMESPACE_END
}  // namespace peppa
