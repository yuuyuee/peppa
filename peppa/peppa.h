/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_PEPPA_H_
#define PEPPA_PEPPA_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Syntax parsing API */

struct pp_parser;
typedef struct pp_parser pp_parser_t;


int pp_parse(pp_parser_t* parser, const char* fname);


/* Virtual machine API */

#ifdef __cplusplus
}
#endif
#endif  /* PEPPA_PEPPA_H_ */
