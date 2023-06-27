/* Copyright 2023 The Peppa Authors. */

#include "peppa/alloc.h"

#include <stdlib.h>
#include <string.h>

static const PeAlloc_Allocator kDefaultAllocator = {
  malloc, realloc, free
};

static const PeAlloc_Allocator* alloc = NULL;

const PeAlloc_Allocator* getAlloc() {
  return alloc ? alloc : &kDefaultAllocator;
}

void setAlloc(const PeAlloc_Allocator* allocator) {
  alloc = allocator;
}

void* PeAlloc_alloc(size_t size) {
  return size > 0 ? getAlloc()->alloc(size) : NULL;
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
      return getAlloc()->realloc(ptr, size);
    PeAlloc_free(ptr);
    return NULL;
  }
  return PeAlloc_alloc(size);
}

void PeAlloc_free(void* ptr) {
  getAlloc()->free(ptr);
}
