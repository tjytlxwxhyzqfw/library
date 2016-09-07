ROOT:=$(shell pwd)/../..
SRC:=$(ROOT)/src/math
LIB:=$(ROOT)/lib

numth:numth_test.c $(SRC)/numth.c $(ROOT)/numth.h
	gcc -Wall -I$(ROOT) -L$(LIB) -Wl,-rpath=$(LIB) -lnumth numth_test.c -o $(ROOT)/numth
clean:
	if [ -f ${ROOT}/numth ];then rm ${ROOT}/numth;fi
