/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_LIST_H_
#define PEPPA_LIST_H_

#include <stddef.h>

typedef void* PeNode[2];

#define _PeList_OFFSET(type, member) ((size_t)&(((type *) 0)->member))

#define PeList_DATA(ptr, type, member)    \
  ((type *) (((void *) (ptr)) - _PeList_OFFSET(type, member)))

#define PeList_NEXT(n) (*(PeNode**) &((*(n))[0]))
#define PeList_PREV(n) (*(PeNode**) &((*(n))[1]))

#define PeList_HEAD(n) (PeList_NEXT(n))

#define PeList_INIT(n) do {                           \
  PeList_NEXT(n) = (n);                               \
  PeList_PREV(n) = (n);                               \
} while (0)

#define PeList_EMPTY(h)                               \
  ((const PeNode *)(h) == (const PeNode *)PeList_NEXT(h))

#define PeList_PREV_NEXT(n) (PeList_NEXT(PeList_PREV(n)))
#define PeList_NEXT_PREV(n) (PeList_PREV(PeList_NEXT(n)))

#define PeList_FOREACH(n, h)                          \
  for ((n) = PeList_NEXT(h); (n) != (h); (n) = PeList_NEXT(n))

#define PeList_INSERT_HEAD(h, n) do {                 \
  PeList_NEXT(n) = PeList_NEXT(h);                    \
  PeList_PREV(n) = (h);                               \
  PeList_NEXT_PREV(n) = (n);                          \
  PeList_NEXT(h) = (n);                               \
} while (0)

#define PeList_INSERT_TAIL(h, n) do {                 \
  PeList_NEXT(n) = (h);                               \
  PeList_PREV(n) = PeList_PREV(h);                    \
  PeList_PREV_NEXT(n) = (n);                          \
  PeList_PREV(h) = (n);                               \
} while (0)

#define PeList_REMOVE(n) do {                         \
  PeList_PREV_NEXT(n) = PeList_NEXT(n);               \
  PeList_NEXT_PREV(n) = PeList_PREV(n);               \
} while (0)

#define PeList_ADD(h, h2) do {                        \
  PeList_PREV_NEXT(h) = PeList_NEXT(h2);              \
  PeList_NEXT_PREV(h2) = PeList_PREV(h2);             \
  PeList_PREV(h) = PeList_PREV(h2);                   \
  PeList_PREV_NEXT(h) = (h);                          \
} while (0)

#define PeList_SPLIT(h, n, h2) do {                   \
  PeList_PREV(h2) = PeList_PREV(h);                   \
  PeList_PREV_NEXT(h2) = (h2);                        \
  PeList_NEXT(h2) = (n);                              \
  PeList_PREV(h) = PeList_PREV(n);                    \
  PeList_PREV_NEXT(h) = (h);                          \
  PeList_PREV(n) = (h2);                              \
} while (0)

#define PeList_MOVE(h, h2) do {                       \
  if (PeList_EMPTY(h)) {                              \
    PeList_INIT(h2);                                  \
  } else {                                            \
    PeNode* n = PeList_HEAD(h);                       \
    PeList_SPLIT(h, n, h2);                           \
  }                                                   \
} while (0)

#endif  /* PEPPA_LIST_H_ */
