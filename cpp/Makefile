ROOT:=$(shell pwd)
ICL:=$(ROOT)/include
SRC:=$(ROOT)/src
LIB:=$(ROOT)/lib

define compile
	g++ -Wall -L$(LIB) -Wl,-rpath=$(LIB) -o $(LIB)/lib${1}.so -shared -fPIC $(SRC)/${2}/${1}.cpp
endef

ALL:=$(LIB)/libnumber.so $(LIB)/libdebug.so $(LIB)/libsegtree.so $(LIB)/liblightseg.so $(LIB)/libprftree.so \
	$(LIB)/liblgnum.so $(LIB)/libhelpers.so

all: $(ALL)

$(LIB)/libnumber.so: $(ICL)/number.h $(SRC)/math/number.cpp
	$(call compile,number,math)

$(LIB)/libdebug.so: $(ICL)/debug.h $(SRC)/utils/debug.cpp
	$(call compile,debug,utils)

$(LIB)/libsegtree.so: $(ICL)/segtree.h $(SRC)/ds/segtree.cpp
	$(call compile,segtree,ds)

$(LIB)/liblightseg.so: $(ICL)/lightseg.h $(SRC)/ds/lightseg.cpp
	$(call compile,lightseg,ds)

$(LIB)/libprftree.so: $(ICL)/prftree.h $(SRC)/string/prftree.cpp
	$(call compile,prftree,string)

$(LIB)/liblgnum.so: $(ICL)/lgnum.h $(SRC)/math/lgnum.cpp
	$(call compile,lgnum,math)

$(LIB)/libhelpers.so: $(ICL)/helpers.h $(SRC)/utils/helpers.cpp
	$(call compile,helpers,utils)

clean:
	rm lib/*
