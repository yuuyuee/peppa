/* Copyright 2023 The Peppa Authors. */

%{

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define YY_USER_INIT
#define YY_USER_ACTION do { yycolumn += yyleng; } while (0);

#define PP_BYPASS_UNTIL(c) while (input(yyscanner) != c) {}
%}

/* option definition from */
%option noyywrap nodefault nomain noreject nostdinit nounput
%option yylineno reentrant
%option debug
%option stack
/* %option bison-bridge bison-locations */
%option perf-report
%option prefix="pp"

%x STRING_LITERAL

%%

%{
  /* before while() on the yylex() */
  char buffer[1024];
  int len = 0;
%}

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
"None"    { /* Operators */
  printf("Keyword or Operator: %*s\n", yyleng, yytext);
}

[a-zA-Z_][a-zA-Z0-9_]*  { /* Identifier */
  printf("Identifier: %*s\n", yyleng, yytext);
}

\"        { /* String literal */
  len = 0;
  yy_push_state(STRING_LITERAL, yyscanner);
}

<STRING_LITERAL>{
  \"            {
    printf("String literal: %*s [%d]\n", len, buffer, len);
    yy_pop_state(yyscanner);
  }

  \n            {
    printf("Unterminated string literal in line %d:%d\n", yylineno, yycolumn + 1);
    yyterminate();
  }

  \\a           { buffer[len++] = '\a'; }
  \\n           { buffer[len++] = '\n'; }
  \\r           { buffer[len++] = '\r'; }
  \\t           { buffer[len++] = '\t'; }
  \\v           { buffer[len++] = '\v'; }
  \\b           { buffer[len++] = '\b'; }
  \\f           { buffer[len++] = '\f'; }
  \\\\          { buffer[len++] = '\\'; }

  \\(.|\n)      {
    memcpy(buffer + len, yytext, yyleng);
    len += yyleng;
  }

  [^\\\n\"]+    {
    memcpy(buffer + len, yytext, yyleng);
    len += yyleng;
  }
}

"#".*     ; /* Comments */

[ \t]*    ; /* Whitespace */

\\\n      |
\\#.*\n   { /* Back-slash */ }

^\n       |
\n        ; /* Newline */

.         { /* Illegal character */
  printf("Unknown charaters \"%*s\"\n", yyleng, yytext);
  yyterminate();
}

<<EOF>>   { /* End of file */
  yyterminate();
}

%%

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
