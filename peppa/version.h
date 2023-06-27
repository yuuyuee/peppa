/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_VERSION_H_
#define PEPPA_VERSION_H_

#define Pe_VERSION_MAJOR 1
#define Pe_VERSION_MINOR 0
#define Pe_VERSION_PATCH 0

#define _Pe_VERSION(a, b, c) ((a << 16) | (b << 8) | (c))
#define Pe_VERSION  \
  _Pe_VERSION(Pe_VERSION_MAJOR, Pe_VERSION_MINOR, Pe_VERSION_PATCH)

#ifndef Pe_STRINGFY
# define _Pe_STRINGFY(s) #s
# define Pe_STRINGFY(s) _Pe_STRINGFY(s)
#endif

#define Pe_VERSION_S                  \
  Pe_STRINGFY(Pe_VERSION_MAJOR) "."   \
  Pe_STRINGFY(Pe_VERSION_MINOR) "."   \
  Pe_STRINGFY(Pe_VERSION_PATCH)

int PeVersion_getVersion();
const char* PeVersion_getVersionString();
const char* PeVersion_getCopyRight();

#endif  /* PEPPA_VERSION_H_ */
