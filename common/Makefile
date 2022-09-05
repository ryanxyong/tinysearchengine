# Makefile for common library
#
# Ryan Yong, April 2021

OBJS = pagedir.o index.o word.o
LIBDIR = ../libcs50
LIB = common.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(LIBDIR)

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h $(LIBDIR)/webpage.h $(LIBDIR)/file.h
index.o: index.h $(LIBDIR)/hashtable.h $(LIBDIR)/counters.h
word.o: word.h

.PHONY: clean

clean:
	rm -f $(LIB)
	rm -f *~ *.o
	rm -rf *.dSYM