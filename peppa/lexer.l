/* Copyright 2023 The Peppa Authors. */

%{

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "peppa/parser.h"

#define YY_USER_INIT
#define YY_USER_ACTION do { yycolumn += yyleng; } while (0);

#define PP_BYPASS_UNTIL(c) while (input(yyscanner) != c) {}
#define PP_HEX2INT(c) (isdigit(c) ? (c) - '0' : ((c) - 'A') + 10)

%}

%option noyywrap nodefault nomain noreject nostdinit
%option noinput nounput
%option reentrant yylineno
%option bison-bridge bison-locations
%option prefix="pp"

%x SINGLE_QUOTE_STR DOUBLE_QUOTE_STR

%%

%{
char buffer[1024];
int len = 0;
%}

<INITIAL>{
  "@"       |
  "="       |
  ","       |
  "("       |
  ")"       |
  "["       |
  "]"       |
  "{"       |
  "}"       |
  "."       |
  ":"       |
  ";"       |
  "~"       |
  "**"      |
  "*"       |
  "/"       |
  "//"      |
  "%"       |
  "+"       |
  "-"       |
  "<<"      |
  ">>"      |
  "&"       |
  "^"       |
  "|"       |
  "in"      |
  "not in"  |
  "is"      |
  "is not"  |
  "<"       |
  "<="      |
  ">"       |
  ">="      |
  "=="      |
  "!="      |
  "not"     |
  "and"     |
  "or"      |
  "if"      |
  "else"    |
  "for"     |
  "match"   |
  "case"    |
  "class"   |
  "True"    |
  "False"   |
  "None"    {
    /* Operators */
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("Keyword or Operator: %s\n", yytext);
  }

  [a-zA-Z_][a-zA-Z0-9_]*  {
    /* Identifier */
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("Identifier: %s\n", yytext);
  }

  \'    {
    /* Single-quoted string literal */
    len = 0;
    BEGIN SINGLE_QUOTE_STR;
  }

  \"    { /* Double-quoted string literal */
    len = 0;
    BEGIN DOUBLE_QUOTE_STR;
  }

  0[bB][01]+    {
    /* Binary integer literal */
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("binary integer literal: %s\n", yytext);
  }

  0[0-7]+   {
    /* Octal integer literal */
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("Octal integer literal: %s\n", yytext);
  }

  0[xX][0-9a-fA-F]+   {
    /* Hexadecimal integer literal */
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("Hexadecimal integer literal: %s\n", yytext);
  }

  [-+]?[0-9]+   {
    /* Decimal integer literal */
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("Decimal integer literal: %s\n", yytext);
  }

  [-+]?[0-9]+("."[0-9]+)?                     |
  [-+]?[0-9]+("."[0-9]+)?([eE][+-]?[0-9]+)?   {
    /* Float integer literal */
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("Float literal: %s\n", yytext);
  }

  "#".*$    {
    /* Comments */
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("Comments: %s\n", yytext);
  }

  [ \t]*  ; /* Whitespace */

  \\[ \t]*(#.*)?\n   {
    /* Back-slash */
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("Back-slash: %s\n", yytext);
  }

  \n    { /* Newline */ }

  <<EOF>> {
    /* End of file */
    yyterminate();
  }
}

<SINGLE_QUOTE_STR>{
  \'    {
    buffer[len] = '\0';
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("String literal: \'%s\' [%d]\n", buffer, len);
    BEGIN INITIAL;
  }

  [^\\']+    {
    memcpy(buffer + len, yytext, yyleng);
    len += yyleng;
  }
}

<DOUBLE_QUOTE_STR>{
  \"    {
    buffer[len] = '\0';
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("String literal: \'%s\' [%d]\n", buffer, len);
    BEGIN INITIAL;
  }

  [^\\"]+    {
    memcpy(buffer + len, yytext, yyleng);
    len += yyleng;
  }
}

<SINGLE_QUOTE_STR,DOUBLE_QUOTE_STR>{
  \\(x|X)[0-9a-fA-F]{1,2}   {
    int value = 0;
    for (int i = 2; i < yyleng; ++i) {
      value <<= 4;
      value += PP_HEX2INT(toupper(yytext[i]));
    }
    buffer[len++] = value & 0xFF;
  }

  \\[0-7]{1,3}    {
    int value = 0;
    for (int i = 1; i < yyleng; ++i) {
      value <<= 3;
      value += yytext[i] - '0';
    }
    buffer[len++] = value & 0xFF;
  }

  \\.   {
    switch (yytext[1]) {
      case '\'':
      case '\"': {
        if ((YY_START == SINGLE_QUOTE_STR && yytext[1] == '\'') ||
            (YY_START == DOUBLE_QUOTE_STR && yytext[1] == '\"')) {
          buffer[len++] = yytext[1];
        } else {
          memcpy(buffer + len, yytext, yyleng);
          len += yyleng;
        }
      }
      break;
      case 'b': buffer[len++] = '\b'; break;
      case 't': buffer[len++] = '\t'; break;
      case 'n': buffer[len++] = '\n'; break;
      case 'v': buffer[len++] = '\v'; break;
      case 'f': buffer[len++] = '\f'; break;
      case 'r': buffer[len++] = '\r'; break;
      default: {
        memcpy(buffer + len, yytext, yyleng);
        len += yyleng;
      }
    }
  }

  <<EOF>>   {
    printf("[%d: %d] ", yylineno, yycolumn);
    printf("Unterminated string literal in line %d:%d\n",
            yylineno, yycolumn);
    yyterminate();
  }
}

<*>.   {
  /* Illegal character */
  printf("[%d: %d] ", yylineno, yycolumn);
  printf("Unknown charaters \"%s\"\n", yytext);
  yyterminate();
}

%%
