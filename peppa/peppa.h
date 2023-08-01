/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_PEPPA_H_
#define PEPPA_PEPPA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Version */
#define PeVer_MAJOR 1
#define PeVer_MINOR 0
#define PeVer_PATCH 0

#define _Pe_VERSION(a, b, c) ((a << 16) | (b << 8) | (c))
#define Pe_VERSION  \
  _Pe_VERSION(PeVer_MAJOR, PeVer_MINOR, PeVer_PATCH)

#ifndef Pe_STRINGFY
# define _Pe_STRINGFY(s) #s
# define Pe_STRINGFY(s) _Pe_STRINGFY(s)
#endif

#define Pe_VERSION_S            \
  Pe_STRINGFY(PeVer_MAJOR) "."  \
  Pe_STRINGFY(PeVer_MINOR) "."  \
  Pe_STRINGFY(Pe_VERSION_PATCH)

int Pe_getVersion();
const char* Pe_getVersionString();
const char* Pe_getCopyRight();


/* State */
typedef struct Pe_State Pe_State;


#ifdef __cplusplus
}
#endif
#endif  /* PEPPA_PEPPA_H_ */
