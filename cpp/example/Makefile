LIB:=${WCC_LIBRARY_PATH}/cpp/lib
ICL:=${WCC_LIBRARY_PATH}/cpp/include

a.out: number.cpp
	g++ -Wall -L$(LIB) -Wl,-rpath=$(LIB) -I$(ICL) -lnumber  number.cpp

.PHONY: clean run asb

asb:
	source-assembler cpp number.cpp	

run:
	make && ./a.out

clean:
	rm a.out

