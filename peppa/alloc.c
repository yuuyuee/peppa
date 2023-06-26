/* Copyright 2023 The Peppa Authors. */

#include "peppa/alloc.h"

#include <stdlib.h>

static const Allocator kDefaultAllocator = {
  malloc, realloc, free
};

static const Allocator* alloc = NULL;

const Allocator* getAlloc() {
  return alloc ? alloc : &kDefaultAllocator;
}

void setAlloc(const Allocator* allocator) {
  alloc = allocator;
}

void* allocMemory(size_t size) {
  return size > 0 ? getAlloc()->alloc(size) : NULL;
}

void* allocMemoryZero(size_t size) {
  void* ptr = allocMemory(size);
  if (ptr)
    memset(ptr, 0, size);
  return ptr;
}

void* reallocMemory(void* ptr, size_t size) {
  if (ptr) {
    if (size > 0)
      return getAlloc()->realloc(ptr, size);
    getAlloc()->free(ptr);
    return NULL;
  }
  return allocMemory(size);
}

void freeMemory(void* ptr) {
  getAlloc()->free(ptr);
}
