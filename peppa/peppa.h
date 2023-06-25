/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_PEPPA_H_
#define PEPPA_PEPPA_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PP_VERSION_MAJOR 1
#define PP_VERSION_MINOR 0
#define PP_VERSION_PATCH 0

#define PP_VERSION_(a, b, c) (((a) << 16) | ((b) << 8) | (c))
#define PP_VERSION  \
  PP_VERSION_(PP_VERSION_MAJOR, PP_VERSION_MINOR, PP_VERSION_PATCH)

#define PPSTR_(x) #x
#define PP_VERSION_STR_(a, b, c) PPSTR_(a) "." PPSTR_(b) "." PPSTR_(c)
#define PP_VERSION_STR  \
  PP_VERSION_STR_(PP_VERSION_MAJOR, PP_VERSION_MINOR, PP_VERSION_PATCH)

#define PP_COPYRIGHT "Copyright 2023 The Peppa Authors"

/* Syntax parsing API */

struct pp_parser;
typedef struct pp_parser pp_parser_t;


int pp_parse(pp_parser_t* parser, const char* fname);


/* Virtual machine API */

#ifdef __cplusplus
}
#endif
#endif  /* PEPPA_PEPPA_H_ */
