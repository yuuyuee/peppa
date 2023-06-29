/* Copyright 2023 The Peppa Authors. */

#include "peppa/alloc.h"

#include <stdlib.h>
#include <string.h>

#include "peppa/macros.h"

static const Pe_Alloc kAllocDefault = {
  malloc, realloc, free
};

static const Pe_Alloc* alloc = NULL;

const Pe_Alloc* Pe_getAlloc() {
  return alloc ? alloc : &kAllocDefault;
}

void Pe_setAlloc(const Pe_Alloc* allocator) {
  alloc = allocator;
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

void* Pe_realloc(void* ptr, size_t size) {
  if (ptr) {
    if (size > 0)
      return Pe_getAlloc()->realloc(ptr, size);
    Pe_free(ptr);
    return NULL;
  }
  return Pe_alloc(size);
}

void Pe_free(void* ptr) {
  Pe_getAlloc()->free(ptr);
}
