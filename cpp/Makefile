ROOT:=$(shell pwd)
ICL:=$(ROOT)/include
SRC:=$(ROOT)/src
LIB:=$(ROOT)/lib

define compile
	g++ -Wall -L$(LIB) -Wl,-rpath=$(LIB) -o $(LIB)/lib${1}.so -shared -fPIC $(SRC)/${2}/${1}.cpp
endef

ALL:=$(LIB)/libnumber.so

all: $(ALL)

$(LIB)/libnumber.so: $(ICL)/number.h $(SRC)/math/number.cpp
	$(call compile,number,math)

clean:
	rm lib/*
