/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_OBJECT_H_
#define PEPPA_OBJECT_H_

#include <stdint.h>

/* State */
typedef struct PeState PeState;

/* Type of integer */
typedef int64_t PeInteger;

/* Type of number */
typedef double PeNumber;

/* Type of function */
typedef int (*PeFunc)(PeState*);

typedef struct PeObject {
  int type;
  union {
    PeInteger integer;
    PeNumber number;
    PeFunc func;
    void* ptr;
  } v;
} PeObject;

#endif  /* PEPPA_OBJECT_H_ */
