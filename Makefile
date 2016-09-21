ROOT:=$(shell pwd)
SRC:=$(ROOT)/src
LIB:=$(ROOT)/lib
CC:=gcc -Wall -L$(LIB) -Wl,-rpath=$(LIB)

ALL:=$(LIB)/libnumth.so $(LIB)/libsegtree.so $(LIB)/libtrie.so $(LIB)/libqueue.so $(LIB)/libheap.so

all:$(ALL)

$(LIB)/libheap.so: heap.h $(SRC)/ds/heap.c
	$(CC) -o lib/libheap.so -shared -fPIC $(SRC)/ds/heap.c

$(LIB)/libqueue.so: queue.h $(SRC)/ds/queue.c
	$(CC) -o lib/libqueue.so -shared -fPIC $(SRC)/ds/queue.c
	
$(LIB)/libtrie.so: trie.h $(SRC)/string/trie.c
	$(CC) -o lib/libtrie.so -shared -fPIC $(SRC)/string/trie.c

$(LIB)/libnumth.so: numth.h $(SRC)/math/numth.c
	$(CC) -o lib/libnumth.so -shared -fPIC $(SRC)/math/numth.c

$(LIB)/libsegtree.so: segtree.h $(SRC)/ds/segtree.c
	$(CC) -o lib/libsegtree.so -shared -fPIC $(SRC)/ds/segtree.c

clean:
	rm lib/*
