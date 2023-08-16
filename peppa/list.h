/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_LIST_H_
#define PEPPA_LIST_H_

#include <stddef.h>

#include "peppa/macros.h"

typedef void* Pe_ListNode[2];

#define PeList_next(n) (*(Pe_ListNode**) &((*(n))[0]))
#define PeList_prev(n) (*(Pe_ListNode**) &((*(n))[1]))
#define PeList_prevNext(n) (PeList_next(PeList_prev(n)))
#define PeList_nextPrev(n) (PeList_prev(PeList_next(n)))

#define PeList_head(n) (PeList_next(n))

#define PeList_init(n) do {                           \
  PeList_next(n) = (n);                               \
  PeList_prev(n) = (n);                               \
} while (0)

#define PeList_empty(h)                               \
  ((const Pe_ListNode*) (h) == PeList_next(h))

#define PeList_foreach(n, h)                          \
  for ((n) = PeList_next(h); (n) != (h); (n) = PeList_next(n))

#define PeList_insertHead(h, n) do {                  \
  PeList_next(n) = PeList_next(h);                    \
  PeList_prev(n) = (h);                               \
  PeList_nextPrev(n) = (n);                           \
  PeList_next(h) = (n);                               \
} while (0)

#define PeList_insertTail(h, n) do {                  \
  PeList_next(n) = (h);                               \
  PeList_prev(n) = PeList_prev(h);                    \
  PeList_prevNext(n) = (n);                           \
  PeList_prev(h) = (n);                               \
} while (0)

#define PeList_remove(n) do {                         \
  PeList_prevNext(n) = PeList_next(n);                \
  PeList_nextPrev(n) = PeList_prev(n);                \
} while (0)

#define PeList_add(h, h2) do {                        \
  PeList_prevNext(h) = PeList_next(h2);               \
  PeList_nextPrev(h2) = PeList_prev(h2);              \
  PeList_prev(h) = PeList_prev(h2);                   \
  PeList_prevNext(h) = (h);                           \
} while (0)

#define PeList_split(h, n, h2) do {                   \
  PeList_prev(h2) = PeList_prev(h);                   \
  PeList_prevNext(h2) = (h2);                         \
  PeList_next(h2) = (n);                              \
  PeList_prev(h) = PeList_prev(n);                    \
  PeList_prevNext(h) = (h);                           \
  PeList_prev(n) = (h2);                              \
} while (0)

#define PeList_move(h, h2) do {                       \
  if (PeList_empty(h)) {                              \
    PeList_init(h2);                                  \
  } else {                                            \
    Pe_ListNode* n = PeList_head(h);                  \
    PeList_split(h, n, h2);                           \
  }                                                   \
} while (0)

#endif  /* PEPPA_LIST_H_ */
