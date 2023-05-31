/* Copyright 2023 The Peppa Authors. */

#ifndef PEPPA_PEPPA_H_
#define PEPPA_PEPPA_H_

/* Syntax parsing API */

#define PP_NAME_MAX 128
typedef struct pp_symbol {
  char name[PP_NAME_MAX];
  struct pp_symbol* next;
} pp_symbol_t;

typedef struct pp_state {
  pp_symbol_t sym;
} pp_state_t;


int pp_parse(const char* fname);


/* Virtual machine API */

#endif  /* PEPPA_PEPPA_H_ */
