/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_INTERNAL_RING_H_
#define PEPPA_INTERNAL_RING_H_

#include <stddef.h>

typedef void* Ring[2];

#define PP_RING_DATA(ptr, type, field)                \
  ((type *) (((void *) (ptr)) - offsetof(type, field)))

#define PP_RING_NEXT(n) (*(Ring**) &((*(n))[0]))
#define PP_RING_PREV(n) (*(Ring**) &((*(n))[1]))

#define PP_RING_HEAD(n) (PP_RING_NEXT(n))

#define PP_RING_INIT(n) do {                          \
  PP_RING_NEXT(n) = (n);                              \
  PP_RING_PREV(n) = (n);                              \
} while (0)

#define PP_RING_EMPTY(h)                              \
  ((const Ring *)(h) == (const Ring *)PP_RING_NEXT(h))

#define PP_RING_PREV_NEXT(n) (PP_RING_NEXT(PP_RING_PREV(n)))
#define PP_RING_NEXT_PREV(n) (PP_RING_PREV(PP_RING_NEXT(n)))

#define PP_RING_FOREACH(n, h)                         \
  for ((n) = PP_RING_NEXT(h); (n) != (h); (n) = PP_RING_NEXT(n))

#define PP_RING_INSERT_HEAD(h, n) do {                \
  PP_RING_NEXT(n) = PP_RING_NEXT(h);                  \
  PP_RING_PREV(n) = (h);                              \
  PP_RING_NEXT_PREV(n) = (n);                         \
  PP_RING_NEXT(h) = (n);                              \
} while (0)

#define PP_RING_INSERT_TAIL(h, n) do {                \
  PP_RING_NEXT(n) = (h);                              \
  PP_RING_PREV(n) = PP_RING_PREV(h);                  \
  PP_RING_PREV_NEXT(n) = (n);                         \
  PP_RING_PREV(h) = (n);                              \
} while (0)

#define PP_RING_REMOVE(n) do {                        \
  PP_RING_PREV_NEXT(n) = PP_RING_NEXT(n);             \
  PP_RING_NEXT_PREV(n) = PP_RING_PREV(n);             \
} while (0)

#define PP_RING_ADD(h, h2) do {                       \
  PP_RING_PREV_NEXT(h) = PP_RING_NEXT(h2);            \
  PP_RING_NEXT_PREV(h2) = PP_RING_PREV(h2);           \
  PP_RING_PREV(h) = PP_RING_PREV(h2);                 \
  PP_RING_PREV_NEXT(h) = (h);                         \
} while (0)

#define PP_RING_SPLIT(h, n, h2) do {                  \
  PP_RING_PREV(h2) = PP_RING_PREV(h);                 \
  PP_RING_PREV_NEXT(h2) = (h2);                       \
  PP_RING_NEXT(h2) = (n);                             \
  PP_RING_PREV(h) = PP_RING_PREV(n);                  \
  PP_RING_PREV_NEXT(h) = (h);                         \
  PP_RING_PREV(n) = (h2);                             \
} while (0)

#define PP_RING_MOVE(h, h2) do {                      \
  if (PP_RING_EMPTY(h)) {                             \
    PP_RING_INIT(h2);                                 \
  } else {                                            \
    Ring* n = PP_RING_HEAD(h);                        \
    PP_RING_SPLIT(h, n, h2);                          \
  }                                                   \
} while (0)

#endif  /* PEPPA_INTERNAL_RING_H_ */
