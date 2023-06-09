/* Copyright 2023 The Peppa Authors. */

/* %expect 0 */

%code top {
/* %code top */

#include <stdio.h>
#include <math.h>

#include "peppa/lexer.h"
#include "peppa/peppa.h"
#include "peppa/object.h"

}

%code requires {
/* %code requires */
#include <stdint.h>
}

%code provides {
/* %code provides */

/////void pperror(YYLTYPE* loc, void*, const char* s);
}

%locations

%define api.prefix {Pe_}
%define api.token.prefix {Pe_TOK_}
%define parse.error verbose
%define api.pure full
%parse-param {pp_state* pp}
%lex-param {void* args}

%union {
  double number;
  int var;
  int fun;
  double exp;
}

%code {
  /* %code unqualified */
}

%token <number> NUM
%token <var> VAR
%token <fun> FUN
%nterm <exp> exp

%destructor {$$ = 0; /* destruct f_val */ } <NUM>

%precedence '='
%left '+' '-'
%left '*' '/'
%precedence NEG
%right '^'

%start input

%%

input
  : /* empty */
  | input line
  ;

line
  : '\n'
  | exp '\n'    { printf("%f\n", $1); }
  | error '\n'  {yyerrok;}
  ;

exp
  : NUM           { $$ = $1; }
  | VAR '=' exp   { $$ = $3; $1 = $3; }
  | FUN '(' exp ')'   { $$ = $1->value.fun($3); }
  | exp '+' exp       { $$ = $1 + $3; }
  | exp '-' exp       { $$ = $1 - $3; }
  | exp '*' exp       { $$ = $1 * $3; }
  | exp '/' exp       { $$ = $1 / $3; }
  | '-' exp %prec NEG   { $$ = -$2; }
  | exp '^' exp       { $$ = pow($1, $3); }
  | '(' exp ')'       {$$ = $2; }
  ;

%%

void pperror(YYLTYPE*, void*, const char* s) {
  printf("%s\n", s);
}