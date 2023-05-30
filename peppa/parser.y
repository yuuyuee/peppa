/* Copyright 2023 The Peppa Authors. */

%{

#include <stdio.h>
#include <stdint.h>

#define YYERROR_VERBOSE 1

void pperror(YYLTYPE* loc, void*, const char* s);
%}

%locations
%define api.prefix {pp}
%define parse.error verbose
%define api.pure true
%parse-param {void* pp}
/* %lex-param {} */
/* #define YYLEX_PARAM */
%start program

%union {
  double f_val;

}

%%

program
  : /* empty */
  ;

%%

void pperror(YYLTYPE*, void*, const char* s) {
  printf("%s\n", s);
}