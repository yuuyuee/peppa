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

struct pp_error_entry {
  int num;
  const char* str;
};

static const struct pp_error_entry error_entries[] = {
  {PP_SYNTAX_ERROR, "Syntax error"},
};

int pp_strerror(int errnum, char* buf, size_t size) {
  const struct pp_error_entry* entry = NULL;
  for (int i = 0; i < PP_ARRAYSIZE(error_entries); ++i) {
    if (errnum == error_entries[i].num) {
      entry = &error_entries[i];
      break;
    }
  }
  int ret = 0;
  if (entry) {
    size_t len = PPMIN(size, strlen(entry->str));
    strncpy(buf, entry->str, len);
    buf[len] = '\0';
  } else {
    ret = strerror_r(errnum, buf, size);
  }
  return ret;
}
