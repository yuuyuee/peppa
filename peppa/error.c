/* Copyright 2023 The Peppa Authors. */

#include "peppa/error.h"
#include "peppa/macros.h"

/* XSI-compliant version of strerror_r */
#ifdef _GNU_SOURCE
# undef _GNU_SOURCE
#endif

#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 600
#endif

#include <string.h>
#include <stdio.h>

static const struct PeErrorEntry {
  int num;
  const char* str;
} error_entries[] = {

};

int PeErr_stringError(int errnum, char* buf, size_t size) {
  const struct PeErrorEntry* entry = NULL;
  for (int i = 0; i < Pe_ARRAYSIZE(error_entries); ++i) {
    if (errnum == error_entries[i].num) {
      entry = &error_entries[i];
      break;
    }
  }
  int ret = 0;
  if (entry) {
    size_t len = Pe_MIN(size, strlen(entry->str));
    strncpy(buf, entry->str, len);
    buf[len] = '\0';
  } else {
    ret = strerror_r(errnum, buf, size);
  }
  return ret;
}
