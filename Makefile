# Copyright 2023 The Peppa Authors.

CFLAGS = -Wall -Wextra  -D_GNU_SOURCE -I. -g
CXXFLAGS = -std=c++11 -Wall -Wextra  -D_GNU_SOURCE -I. -g
LDFLAGS = -lpthread

all: list_test;

lexer: peppa/lexer.c peppa/lexer.h parser
	gcc -std=c99 -D_GNU_SOURCE -Wall -Wextra -I. -g -o $@ $<

peppa/lexer.h peppa/lexer.c: peppa/lexer.l
	flex --debug --perf-report --header-file=$*.h -o $*.c $<

parser: peppa/parser.h peppa/parser.c;

peppa/parser.h peppa/parser.c: peppa/parser.y
	bison --debug --verbose --report=all --defines=$*.h -o $*.c $<

list_test: peppa/list_test.cc third_party/gtest/gtest_main.cc third_party/gtest/gmock-gtest-all.cc
	g++ $(CXXFLAGS) -I./third_party/gtest -o $@ $^ $(LDFLAGS)

clean:
	rm -rf peppa/lexer.c peppa/lexer.h \
		peppa/parser.h peppa/parser.c peppa/parser.output \
		lexer list_test

.PHONY: parser lexer
