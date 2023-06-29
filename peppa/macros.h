/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_MACROS_H_
#define PEPPA_MACROS_H_

#include <stddef.h>

#define Pe_ARRAYSIZE(a) (sizeof(a) / sizeof(*(a)))

#define Pe_MAX(a, b) ((a) > (b) ? (a) : (b))
#define Pe_MAX3(a, b, c) Pe_MAX(Pe_MAX(a, b), c)
#define Pe_MIN(a, b) ((a) < (b) ? (a) : (b))
#define Pe_MIN3(a, b, c) Pe_MIN(Pe_MIN(a, b), c)

#define Pe_SWAP(type, a, b) do {  \
  type tmp = b;                   \
  b = a;                          \
  a = tmp;                        \
} while (0)

#ifndef Pe_STRINGFY
# define _Pe_STRINGFY(s) #s
# define Pe_STRINGFY(s) _Pe_STRINGFY(s)
#endif

#ifndef Pe_JOIN
# define _Pe_JOIN(a, b) a ## b
# define Pe_JOIN(a, b) _Pe_JOIN(a, b)
#endif

#define Pe_ALIGIN(n, a) (((n) + (a) - 1) & ~((a) - 1))
#define Pe_COMPARE(a, b) (((a) > (b)) - ((a) < (b)))
#define Pe_CAST(t, p) ((t) (p))

#endif  /* PEPPA_MACROS_H_ */
