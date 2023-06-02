/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_PEPPA_H_
#define PEPPA_PEPPA_H_

/* Syntax parsing API */

struct pp_parser;
typedef struct pp_parser pp_parser_t;


int pp_parse(pp_parser_t* parser, const char* fname);


/* Virtual machine API */

#endif  /* PEPPA_PEPPA_H_ */