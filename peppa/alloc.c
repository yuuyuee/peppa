/* Copyright 2023 The Peppa Authors. */

#include "peppa/alloc.h"

#include <stdlib.h>
#include <string.h>

#include "peppa/error.h"

static const Pe_Alloc kAllocDefault = {
  malloc, free
};

static const Pe_Alloc* _alloc = NULL;

const Pe_Alloc* Pe_getAlloc() {
  return _alloc ? _alloc : &kAllocDefault;
}

void Pe_setAlloc(const Pe_Alloc* alloc) {
  _alloc = alloc;
}

void* Pe_alloc(size_t size) {
  PE_CHECK2(size > 0);
  return Pe_getAlloc()->alloc(size);
}

void* Pe_allocz(size_t size) {
  void* ptr = Pe_alloc(size);
  memset(ptr, 0, size);
  return ptr;
}

void Pe_free(void* ptr) {
  Pe_getAlloc()->free(ptr);
}
