ROOT:=$(shell pwd)
ICL:=$(ROOT)/include
SRC:=$(ROOT)/src
LIB:=$(ROOT)/lib

define compile
	gcc -Wall -L$(LIB) -Wl,-rpath=$(LIB) -o $(LIB)/lib${1}.so -shared -fPIC $(SRC)/${2}/${1}.c
endef

ALL:=$(LIB)/libnumth.so $(LIB)/libsegtree.so $(LIB)/libtrie.so $(LIB)/libqueue.so $(LIB)/libheap.so

all: $(ALL)

$(LIB)/libheap.so: $(ICL)/heap.h $(SRC)/ds/heap.c
	$(call compile,heap,ds)

$(LIB)/libqueue.so: $(ICL)/queue.h $(SRC)/ds/queue.c
	$(call compile,queue,ds)
	
$(LIB)/libtrie.so: $(ICL)/trie.h $(SRC)/string/trie.c
	$(call compile,trie,string)

$(LIB)/libnumth.so: $(ICL)/numth.h $(SRC)/math/numth.c
	$(call compile,numth,math)

$(LIB)/libsegtree.so: $(ICL)/segtree.h $(SRC)/ds/segtree.c
	$(call compile,segtree,ds)

clean:
	rm lib/*
