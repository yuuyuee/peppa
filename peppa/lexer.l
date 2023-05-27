/* Copyright 2023 The Peppa Authors. */

%{

#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define YY_USER_INIT
#define YY_USER_ACTION do { yycolumn += yyleng; } while (0);

%}

/* option definition from */
%option noyywrap nodefault nomain noreject nostdinit nounput
%option yylineno reentrant
%option bison-bridge bison-locations
%option perf-report
%option prefix=pp
%pointer

/* start conditions */
%x IMPORT

DIGIT [0-9]
WS [ \t]

%{
  // printf("on declartion statement\n");
#define MAX_FILES 6
  struct Context {
    char filename[PATH_MAX][MAX_FILES];
    int current_filename;
  };

  void InitContext(struct Context* context);
  void PushFile(yyscan_t scanner, const char* fname);
  void PopFile(yyscan_t scanner);
%}

%%

%{
  // printf("on rule statement\n");
%}

^import BEGIN IMPORT;

<IMPORT>{
  [ \t]*  { /* eat the whiltespace */ }

  [^ \t\n#]+ {
    while (input(yyscanner) != '\n') {}
    yycolumn = 1;
    PushFile(yyscanner, yytext);
    BEGIN INITIAL;
  }

  .|\n {
    printf("bad import in line %d:%d\n", yylineno, yycolumn);
    yyterminate();
  }

}

"#". {
  while (input(yyscanner) != '\n') {}
}

{WS}+ {}

{DIGIT}+ {
  struct Context* context = (struct Context*) yyget_extra(yyscanner);
  printf("Recognized symbol at file '%s' line %d:%d: %s\n",
          context->filename[context->current_filename], yylineno, yycolumn, yytext);
}

[a-z]+ {
  struct Context* context = (struct Context*) yyget_extra(yyscanner);
  printf("Recognized symbol at file '%s' line %d:%d: %s\n",
          context->filename[context->current_filename], yylineno, yycolumn, yytext);
}

^\n |
\n  { yycolumn = 1; }

. {}

<<EOF>> {
  PopFile(yyscanner);
  struct Context* context = (struct Context*) yyget_extra(yyscanner);
  if (context->current_filename < 0)
    yyterminate();
}

%%

void InitContext(struct Context* context) {
  memset(context, 0, sizeof(*context));
  context->current_filename = -1;
}

void PushFile(yyscan_t scanner, const char* fname) {
  struct Context* context = (struct Context*) yyget_extra(scanner);
  assert(context != NULL);
  assert(context->current_filename + 1 < MAX_FILES);

  context->current_filename += 1;
  memcpy(context->filename[context->current_filename],
         fname, strlen(fname));
  FILE* fp = fopen(fname, "rb");
  assert(fp != NULL);
  YY_BUFFER_STATE bs = yy_create_buffer(fp, YY_BUF_SIZE, scanner);
  yypush_buffer_state(bs, scanner);
}

void PopFile(yyscan_t scanner) {
  struct Context* context = (struct Context*) yyget_extra(scanner);
  assert(context != NULL);
  context->current_filename -= 1;
  yypop_buffer_state(scanner);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
      printf("Usage: %s file\n", argv[0]);
      exit(EXIT_FAILURE);
  }

  struct Context context;
  InitContext(&context);

  yyscan_t scanner;
  yylex_init_extra(&context, &scanner);
  yyset_debug(1, scanner);
  PushFile(scanner, argv[1]);
  while (yylex(scanner) != 0) {}
  PopFile(scanner);

  yylex_destroy(scanner);

  return 0;
}
