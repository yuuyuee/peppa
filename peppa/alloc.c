/* Copyright 2023 The Peppa Authors. */

#include "peppa/alloc.h"

#include <stdlib.h>
#include <string.h>

static const PeAllocator kDefaultAllocator = {
  malloc, realloc, free
};

static const PeAllocator* alloc = NULL;

const PeAllocator* PeAlloc_getAlloc() {
  return alloc ? alloc : &kDefaultAllocator;
}

void PeAlloc_setAlloc(const PeAllocator* allocator) {
  alloc = allocator;
}

void* PeAlloc_alloc(size_t size) {
  return size > 0 ? PeAlloc_getAlloc()->alloc(size) : NULL;
}

void* PeAlloc_allocz(size_t size) {
  void* ptr = PeAlloc_alloc(size);
  if (ptr)
    memset(ptr, 0, size);
  return ptr;
}

void* PeAlloc_realloc(void* ptr, size_t size) {
  if (ptr) {
    if (size > 0)
      return PeAlloc_getAlloc()->realloc(ptr, size);
    PeAlloc_free(ptr);
    return NULL;
  }
  return PeAlloc_alloc(size);
}

void PeAlloc_free(void* ptr) {
  PeAlloc_getAlloc()->free(ptr);
}
