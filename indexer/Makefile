# Makefile for indexer
#
# Ryan Yong, May 2021

PROG = indexer
PROG2 = indextest
OBJS = indexer.o
OBJS2  = indextest.o
LIBDIR = ../libcs50
COMMONDIR = ../common
LIB = $(COMMONDIR)/common.a $(LIBDIR)/libcs50-given.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(LIBDIR) -I$(COMMONDIR)

.PHONY: all

all: $(PROG) $(PROG2)


$(PROG): $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $^ -o $@

$(PROG2): $(OBJS2) $(LIB)
	$(CC) $(CFLAGS) $^ -o $@

indexer.o: $(LIBDIR)/webpage.h $(COMMONDIR)/index.h $(COMMONDIR)/word.h $(COMMONDIR)/pagedir.h $(LIBDIR)/file.h
indextest.o: $(LIBDIR)/file.h $(LIBDIR)/counters.h $(COMMONDIR)/index.h


.PHONY: test

test: $(PROG)
	bash -v testing.sh

.PHONY: clean

clean:
	rm -f $(PROG)
	rm -f $(PROG2)
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f vgcore.*
	rm -rf testOut
	rm -f core