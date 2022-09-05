# Makefile for querier
#
# Ryan Yong, May 2021

PROG = querier
PROG2 = fuzzquery
OBJS = querier.o
OBJS2 = fuzzquery.o
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

querier.o: $(COMMONDIR)/pagedir.h $(COMMONDIR)/index.h $(LIBDIR)/file.h $(COMMONDIR)/word.h $(LIBDIR)/hashtable.h $(LIBDIR)/counters.h
fuzzquery.o: $(LIBDIR)/mem.h $(LIBDIR)/file.h


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
	rm -f core