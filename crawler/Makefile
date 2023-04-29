# Makefile for 'crawler'
#
# Selena Zhou, 23S

C = ../common
L = ../libcs50
LIBS =
LLIBS = $C/common.a $L/libcs50.a

PROG = crawler
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all		# for memory-leak tests

all: crawler

crawler: crawler.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: $L/webpage.h $C/pagedir.h $L/mem.h $L/bag.h $L/hashtable.h

.PHONY: test valgrind clean

test: testing.sh
	bash -v testing.sh >& testing.out

valgrind: crawler #testing.sh
	$(VALGRIND) ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ pgdir 3

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f ~* *.o
	rm -f crawler
	rm -f core