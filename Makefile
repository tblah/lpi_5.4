COMPILE_OPTS= -g -Wall

all: dup dup2

dup: dup.c
	cc $(COMPILE_OPTS) -o dup dup.c

dup2: dup2.c
	cc $(COMPILE_OPTS) -o dup2 dup2.c

clean:
	rm dup dup2
