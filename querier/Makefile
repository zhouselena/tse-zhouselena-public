# Makefile for 'querier' module
# Selena Zhou, 23S

OBJS = querier.o
C = ../common
L = ../libcs50
LIBS =
LLIBS = $C/common.a $L/libcs50.a


CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: querier

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: $L/webpage.h $C/pagedir.h $C/word.h $C/index.h $L/mem.h $L/bag.h $L/hashtable.h $L/set.h $L/counters.h

.PHONY: test valgrind clean

test: testing.sh
	bash -v testing.sh >& testing.out

valgrind: querier
	$(VALGRIND) ./querier ../data/pgdir/letters ../data/indexer/pgdir-letters-index

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f ~* *.o
	rm -f querier
	rm -f core
