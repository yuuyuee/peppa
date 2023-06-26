/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_ALLOC_H_
#define PEPPA_ALLOC_H_

#include <stddef.h>

typedef struct Allocator {
  /* Allocates size bytes and returns a pointer to the allocated memory,
   * if size is 0 or error occured, return NULL. */
  void* (*alloc)(size_t size);

  /* Changes the size of the memory pointed to by ptr to size bytes, the
   * contexts will be unchanged in the range from the start of the region
   * up to the minimum of the old and new sizes. If ptr is NULL, then the
   * call is equivalent to "alloc"; if size is equal to 0 and ptr is not
   * NULL, then the call is equivalent to "free". */
  void* (*realloc)(void* ptr, size_t size);

  /* Frees the memory space pointed to by ptr. */
  void (*free)(void* ptr);
} Allocator;

const Allocator* getAlloc();
void setAlloc(const Allocator* allocator);

/* Wrapped functions */

/* Allocates size bytes and returns a pointer to the allocated memory. */
void* allocMemory(size_t size);

/* Like as "allocMemory" and the memory is set to zero. */
void* allocMemoryZero(size_t size);

/* Changes the size of the memory pointed to by ptr to size bytes. */
void* reallocMemory(void* ptr, size_t size);

/* Frees the memory space. */
void freeMemory(void* ptr);

#define PPALLOC(type) ((type*) allocMemory(sizeof(type)))
#define PPALLOCN(type, n) ((type*) allocMemory(sizeof(type) * (n)))
#define PPALLOCZ(type) ((type*) allocMemoryZero(sizeof(type)))
#define PPALLOCZN(type, n) ((type*) allocMemoryZero(sizeof(type) * (n)))

#endif  /* PEPPA_ALLOC_H_ */
