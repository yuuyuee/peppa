/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_VERSION_H_
#define PEPPA_VERSION_H_

#define PeVer_MAJOR 1
#define PeVer_MINOR 0
#define PeVer_PATCH 0

#define _PeVer_VERSION(a, b, c) ((a << 16) | (b << 8) | (c))
#define PeVer_VERSION  \
  _PeVer_VERSION(PeVer_MAJOR, PeVer_MINOR, PeVer_PATCH)

#ifndef Pe_STRINGFY
# define _Pe_STRINGFY(s) #s
# define Pe_STRINGFY(s) _Pe_STRINGFY(s)
#endif

#define PeVer_VERSION_S         \
  Pe_STRINGFY(PeVer_MAJOR) "."  \
  Pe_STRINGFY(PeVer_MINOR) "."  \
  Pe_STRINGFY(Pe_VERSION_PATCH)

int PeVer_getVersion();
const char* PeVer_getVersionString();
const char* PeVer_getCopyRight();

#endif  /* PEPPA_VERSION_H_ */
