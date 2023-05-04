# Makefile for 'common'
# 
# This makefile will default build the common.a library.
# It will also have a clean target that removes files produced by Make.
#
# Selena Zhou, 23S

OBJS = pagedir.o word.o index.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h ../libcs50/webpage.h ../libcs50/mem.h
word.o: word.h
index.o: index.h ../libcs50/hashtable.h ../libcs50/counters.h ../libcs50/file.h

.PHONY: all clean

clean:
	rm -f core
	rm -f $(LIB) *~ *.o
