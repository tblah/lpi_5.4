COMPILE_OPTS= -g -Wall

all: dup dup2

dup: dup.c testDup.o testDup.h
	cc $(COMPILE_OPTS) -o dup dup.c testDup.h testDup.o

dup2: dup2.c testDup.o testDup.h
	cc $(COMPILE_OPTS) -o dup2 dup2.c testDup.h testDup.o

testdup.o: testDup.c testDup.h
	cc $(COMPILE_OPTS) testDup.c

clean:
	rm dup dup2 *.o
