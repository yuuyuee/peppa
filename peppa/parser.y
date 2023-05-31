/* Copyright 2023 The Peppa Authors. */

/* %expect 0 */

%code top {
/* Emitted on top of the implementation file. */

#include <stdio.h>

#include "peppa/lexer.h"
#include "peppa/peppa.h"
}

%code requires {
/* Emitted in the header file, before the definition of YYSTYPE. */
#include <stdint.h>
}

%code provides {
/* Emitted in the header file, after the definition of YYSTYPE. */

void pperror(YYLTYPE* loc, void*, const char* s);
}

%locations

%define api.prefix {pp}
%define api.token.prefix {TOK_}
%define parse.error verbose
%define api.pure full
%parse-param {pp_state* pp}
%lex-param {void* args}

%union {
  double f_val;
  uint64_t i_val;
  char* s_val;
  pp_symbol_t* id;
}

%token <f_val> FLT
%token <i_val> INT
%token <s_val> STR
%token <id> ID


%left '+' '-'
%left '*' '/'

%start program

%%

program
  : /* empty */
  ;

%%

void pperror(YYLTYPE*, void*, const char* s) {
  printf("%s\n", s);
}