ROOT:=$(shell pwd)
SRC:=$(ROOT)/src
LIB:=$(ROOT)/lib

CC:=gcc -Wall -L$(LIB) -Wl,-rpath=$(LIB)
ALL:=$(LIB)/libnumth.so $(LIB)/libsegtree.so $(LIB)/libtrie.so

all:$(ALL)

$(LIB)/libtrie.so: trie.h $(SRC)/string/trie.c
	$(CC) -o lib/libtrie.so -shared -fPIC $(SRC)/string/trie.c

$(LIB)/libnumth.so: numth.h $(SRC)/math/numth.c
	$(CC) -o lib/libnumth.so -shared -fPIC $(SRC)/math/numth.c

$(LIB)/libsegtree.so: segtree.h $(SRC)/ds/segtree.c
	$(CC) -o lib/libsegtree.so -shared -fPIC $(SRC)/ds/segtree.c

clean:
	rm lib/*
