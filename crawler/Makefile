# Makefile for crawler
#
# Ryan Yong, April 2021

PROG = crawler
OBJS = crawler.o
LIBDIR = ../libcs50
COMMONDIR = ../common
LIB = ../libcs50/libcs50-given.a ../common/common.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(LIBDIR) -I$(COMMONDIR)

$(PROG): $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $^ -o $@

crawler.o: $(LIBDIR)/hashtable.h $(LIBDIR)/bag.h $(LIBDIR)/webpage.h $(COMMONDIR)/pagedir.h
hashtable.o: $(LIBDIR)/hashtable.h
bag.o: $(LIBDIR)/bag.h
webpage.o: $(LIBDIR)/webpage.h

.PHONY: test

test: $(PROG)
	bash -v testing.sh &> testing.out

.PHONY: valgrind

valgrind: $(PROG)
	bash -v valgrind.sh &> valTesting.out

.PHONY: clean

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -rf testOut