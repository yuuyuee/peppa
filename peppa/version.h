/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_VERSION_H_
#define PEPPA_VERSION_H_

#define PP_VERSION_MAJOR 1
#define PP_VERSION_MINOR 0
#define PP_VERSION_PATCH 0

#define PP_VERSION_(a, b, c) ((a << 16) | (b << 8) | (c))
#define PP_VERSION  \
  PP_VERSION_(PP_VERSION_MAJOR, PP_VERSION_MINOR, PP_VERSION_PATCH)

#ifndef PPSTR
# define PPSTR(s) PP_STR_(s)
# define PPSTR_(s) #s
#endif

#define PP_VERSION_STR          \
  PPSTR(PP_VERSION_MAJOR) "."   \
  PPSTR(PP_VERSION_MINOR) "."   \
  PPSTR(PP_VERSION_PATCH)

#endif  /* PEPPA_VERSION_H_ */
