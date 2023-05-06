# Makefile for 'indexer' module
# Selena Zhou, 23S

OBJS = indexer.o indextest.o
C = ../common
L = ../libcs50
LIBS =
LLIBS = $C/common.a $L/libcs50.a


CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: indexer indextest

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest.o: indexer.c

indexer.o: $L/webpage.h $C/pagedir.h $C/word.h $C/index.h $L/mem.h $L/bag.h $L/hashtable.h $L/set.h $L/counters.h

.PHONY: test valgrind clean

test: testing.sh
	bash -v testing.sh >& testing.out

valgrind: indexer
	$(VALGRIND) ./indexer ../data/pgdir/letters ../data/indexer/pgdir-letters-index

# valgrind: indextest
# 	$(VALGRIND) ./indextest ../data/indexer/pgdir-letters-index ../data/indexer/indextest/it-pgdir-letters-index

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f ~* *.o
	rm -f indextest
	rm -f indexer
	rm -f core
