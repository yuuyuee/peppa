/* Copyright 2023 The Peppa Authors. */

#include "peppa/error.h"

/* XSI-compliant version of strerror_r */
#ifdef _GNU_SOURCE
# undef _GNU_SOURCE
#endif

#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 600
#endif

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#include "peppa/macros.h"
#include "peppa/format.h"

static const struct Pe_ErrorEntry {
  int num;
  const char* str;
} error_entries[] = {
  { PeErr_SYNTAX_ERROR, "Syntax error" },
};

const char* Pe_strError(int errnum, char* buf, size_t size) {
  const struct Pe_ErrorEntry* entry = NULL;
  for (int i = 0; i < Pe_ARRAYSIZE(error_entries); ++i) {
    if (errnum == error_entries[i].num) {
      entry = &error_entries[i];
      break;
    }
  }
  if (entry) {
    size_t len = Pe_MIN(size, strlen(entry->str));
    strncpy(buf, entry->str, len);
    buf[len] = '\0';
  } else {
    int save_errno = errno;
    int ret = strerror_r(errnum, buf, size);
    if (ret != 0) {
      PE_CHECK2(errno == EINVAL);
      Pe_format(buf, size, "Unknown error %d", errnum);
    }
    errno = save_errno;
  }
  return buf;
}

void _Pe_error(const char* file, int line, const char* fun,
               const char* fmt, ...) {
  const char* ptr = strrchr(file, '/');
  if (ptr)
    file = ptr + 1;

#define Pe_BSZ 1024
  char buf[Pe_BSZ];
  size_t n =
      Pe_format(buf, Pe_BSZ, "Error on the %s:%d <%s>: ", file, line, fun);

  va_list ap;
  va_start(ap, fmt);
  Pe_formatv(buf + n, Pe_BSZ - n, fmt, ap);
  va_end(ap);
}
