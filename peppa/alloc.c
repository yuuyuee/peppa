/* Copyright 2023 The Peppa Authors. */

#include "peppa/alloc.h"

#include <stdlib.h>
#include <string.h>

static const Pe_Alloc kAllocDefault = {
  malloc, free
};

static const Pe_Alloc* _Pe_alloc = NULL;

const Pe_Alloc* Pe_getAlloc() {
  return _Pe_alloc ? _Pe_alloc : &kAllocDefault;
}

void Pe_setAlloc(const Pe_Alloc* alloc) {
  _Pe_alloc = alloc;
}

void* Pe_alloc(size_t size) {
  return size > 0 ? Pe_getAlloc()->alloc(size) : NULL;
}

void* Pe_allocz(size_t size) {
  void* ptr = Pe_alloc(size);
  if (ptr)
    memset(ptr, 0, size);
  return ptr;
}

void Pe_free(void* ptr) {
  Pe_getAlloc()->free(ptr);
}
