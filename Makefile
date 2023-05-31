# Copyright 2023 The Peppa Authors.

all: lexer parser;

lexer: peppa/lexer.c peppa/lexer.h parser
	gcc -std=c99 -D_GNU_SOURCE -Wall -Wextra -I. -g -o $@ $<

peppa/lexer.h peppa/lexer.c: peppa/lexer.l
	flex --debug --perf-report --header-file=$*.h -o $*.c $<

parser: peppa/parser.h peppa/parser.c;

peppa/parser.h peppa/parser.c: peppa/parser.y
	bison --debug --verbose --report=all --defines=$*.h -o $*.c $<

clean:
	rm -rf peppa/lexer.c peppa/lexer.h \
		peppa/parser.h peppa/parser.c peppa/parser.output \
		lexer

.PHONY: parser lexer
