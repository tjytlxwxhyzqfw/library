ROOT:=$(shell pwd)
SRC:=$(ROOT)/src
LIB:=$(ROOT)/lib
TEST:=$(ROOT)/tests

CC:=gcc -Wall -L$(LIB) -Wl,-rpath=$(LIB)

ALL:=$(LIB)/libnumth.so

wcc:
	export ROOT
	cd $(SRC)/math && make

all:$(ALL)

$(LIB)/libnumth.so: numth.h $(SRC)/math/numth.c
	$(CC) -o lib/libnumth.so -shared -fPIC $(SRC)/math/numth.c

clean:
	rm lib/*

test:all
	cd $(TEST)/math && make
test-clean:
	cd $(TEST)/math && make clean

