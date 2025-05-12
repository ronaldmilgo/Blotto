# Variables
# variables for compiling rules
SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall -pedantic -std=c17 -g3 

# paths for testing/submitting
HW4=/c/cs223/hw4
BIN=/c/cs223/bin


# Compiling

all: Blotto GmapUnit


Blotto: blotto.o entry.o string_key.o gmap_array.o
	${CC} ${CCFLAGS} -o $@ $^ -lm


GmapUnit: gmap_unit.o gmap_array.o gmap_test_functions.o string_key.o 
	${CC} ${CCFLAGS} -o $@ $^ -lm

blotto.o: entry.h gmap.h string_key.h
entry.o: entry.h 
gmap_array.o: gmap.h
gmap_test_functions.o: gmap_test_functions.h
string_key.o: string_key.h
gmap_unit.o: gmap.h gmap_test_functions.h string_key.h



clean:
	rm -f Blotto Unit *.o


test:
	${HW4}/Tests/test.Blotto


singletest:
	${HW4}/Tests/$(TEST)


submit:
	${BIN}/submit 4 makefile blotto.c gmap_array.c entry.c entry.h log

check:
	${BIN}/check 4

testit:
	${BIN}/testit 4 Blotto

