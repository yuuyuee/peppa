# Copyright 2023 The Peppa Authors.

all: peppa/lexer.c peppa/lexer.h
	gcc -std=c99 -D_GNU_SOURCE -Wall -Wextra -g -o lexer $^

peppa/lexer.h peppa/lexer.c: peppa/lexer.l
	flex --header-file=$*.h -o $*.c $<

clean:
	rm -rf peppa/lexer.c peppa/lexer.h lexer
