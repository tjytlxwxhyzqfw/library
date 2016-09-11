ROOT:=$(shell pwd)
SRC:=$(ROOT)/src
LIB:=$(ROOT)/lib

CC:=gcc -Wall -L$(LIB) -Wl,-rpath=$(LIB)
ALL:=$(LIB)/libnumth.so $(LIB)/libsegtree.so

all:$(ALL)

$(LIB)/libnumth.so: numth.h $(SRC)/math/numth.c
	$(CC) -o lib/libnumth.so -shared -fPIC $(SRC)/math/numth.c

$(LIB)/libsegtree.so: segtree.h $(SRC)/ds/segtree.c
	$(CC) -o lib/libsegtree.so -shared -fPIC $(SRC)/ds/segtree.c

clean:
	rm lib/*
