ROOT:=$(shell pwd)
ICL:=$(ROOT)/include
SRC:=$(ROOT)/src
LIB:=$(ROOT)/lib

define compile
	g++ -Wall -L$(LIB) -Wl,-rpath=$(LIB) -o $(LIB)/lib${1}.so -shared -fPIC $(SRC)/${2}/${1}.cpp
endef

ALL:=$(LIB)/libnumber.so $(LIB)/libarraytree.so $(LIB)/libdebug.so $(LIB)/libsegtree.so $(LIB)/libprftree.so

all: $(ALL)

$(LIB)/libnumber.so: $(ICL)/number.h $(SRC)/math/number.cpp
	$(call compile,number,math)

$(LIB)/libarraytree.so: $(ICL)/arraytree.h $(SRC)/utils/arraytree.cpp
	$(call compile,arraytree,utils)

$(LIB)/libdebug.so: $(ICL)/debug.h $(SRC)/utils/debug.cpp
	$(call compile,debug,utils)

$(LIB)/libsegtree.so: $(ICL)/segtree.h $(SRC)/ds/segtree.cpp
	$(call compile,segtree,ds)

$(LIB)/libprftree.so: $(ICL)/prftree.h $(SRC)/string/prftree.cpp
	$(call compile,prftree,string)

clean:
	rm lib/*
