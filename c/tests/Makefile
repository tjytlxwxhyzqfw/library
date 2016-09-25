ROOT:=$(shell pwd)/..
SRC:=$(ROOT)/src/
LIB:=$(ROOT)/lib

segtree:ds/segtree_test.c $(SRC)/ds/segtree.c $(ROOT)/segtree.h
	gcc -Wall -I$(ROOT) -L$(LIB) -Wl,-rpath=$(LIB) -lsegtree ds/segtree_test.c -o segtree

numth:math/numth_test.c $(SRC)/math/numth.c $(ROOT)/numth.h
	gcc -Wall -I$(ROOT) -L$(LIB) -Wl,-rpath=$(LIB) -lnumth math/numth_test.c -o numth
clean:
	if [ -f numth ];then rm numth;fi
	if [ -f segtree ];then rm segtree;fi

