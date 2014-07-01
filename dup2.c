#include <stdio.h>		// for printf and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit

// implement int dup2(int oldfd, int newfd) -> returns new dup-ed fd on succsess or -1 on error
// makes duplicate of oldfd to an fd = newfd. 
// if newfd is already open then this is closed by dup2 (ignore errors scilently)
// if oldfd is invalid then fail with errno = EBADF and newfd is not closed
// if oldfd is valid and oldfd = newfd then do nothing and return oldfd. 

typedef enum {false, true} bool;

void errExit(char* errMsg)
{
	int errorno = errno;

	printf("Exiting on error:\t%s\n", errMsg);
	printf("\n\nThe errno was %i\n", errorno);
	
	fflush(stdout);

	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
	

	exit(EXIT_SUCCESS);
}
