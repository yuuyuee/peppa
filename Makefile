# Copyright 2023 The Peppa Authors.

CPPFLAGS = -D_GNU_SOURCE -I$(CURDIR) -I$(CURDIR)/third_party/gtest
CFLAGS = -Wall -Wextra -g
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LDFLAGS = -lpthread

GTEST_CORE = $(CURDIR)/third_party/gtest/gmock-gtest-all.cc
GTEST_MAIN = $(CURDIR)/third_party/gtest/gtest_main.cc

all: list_test;

lexer: peppa/lexer.c peppa/lexer.h;
#	gcc -std=c99 -D_GNU_SOURCE -Wall -Wextra -I. -g -o $@ $<

peppa/lexer.h peppa/lexer.c: peppa/lexer.l
	flex --debug --perf-report --header-file=$*.h -o $*.c $<

parser: peppa/parser.h peppa/parser.c;

peppa/parser.h peppa/parser.c: peppa/parser.y
	bison --debug --verbose --report=all --defines=$*.h -o $*.c $<

list_test: peppa/list_test.cc $(GTEST_CORE) $(GTEST_MAIN)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf peppa/lexer.c peppa/lexer.h \
		peppa/parser.h peppa/parser.c peppa/parser.output \
		lexer list_test

.PHONY: parser lexer
