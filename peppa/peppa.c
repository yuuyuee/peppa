/* Copyright 2023 The Peppa Authors. */

#include "peppa/peppa.h"

#include <stdio.h>
#include <stdlib.h>



int pp_parse(const char* fname) {

}

int main(int argc, char* argv[]) {
  if (argc != 2) {
      printf("Usage: %s file\n", argv[0]);
      exit(EXIT_FAILURE);
  }

  const char* fname = argv[1];
  FILE* fp = fopen(fname, "r");
  if (!fp) {
    printf("open file failed: %s\n", fname);
    exit(EXIT_FAILURE);
  }

  yyscan_t scanner;
  yylex_init_extra(0, &scanner);
  yyset_debug(1, scanner);
  yyset_in(fp, scanner);
  while (yylex(scanner) != 0) {}
  yylex_destroy(scanner);
  fclose(fp);

  return 0;
}
