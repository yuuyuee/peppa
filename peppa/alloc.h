/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_ALLOC_H_
#define PEPPA_ALLOC_H_

#include <stddef.h>

typedef struct PeAllocator {
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
} PeAllocator;

const PeAllocator* PeAlloc_getAlloc();
void PeAlloc_setAlloc(const PeAllocator* allocator);

/* Wrapped functions */

/* Allocates size bytes and returns a pointer to the allocated memory. */
void* PeAlloc_alloc(size_t size);

/* Like as "PeAlloc_alloc" but the memory is set to zero. */
void* PeAlloc_allocz(size_t size);

/* Changes the size of the memory pointed to by ptr to size bytes. */
void* PeAlloc_realloc(void* ptr, size_t size);

/* Frees the memory space. */
void PeAlloc_free(void* ptr);

#define Pe_ALLOC(type, n) ((type*) PeAlloc_alloc(sizeof(type) * (n)))
#define Pe_ALLOC_Z(type, n) ((type*) PeAlloc_allocz(sizeof(type) * (n)))

#endif  /* PEPPA_ALLOC_H_ */
