#include <stdio.h>		// for printf and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit

// implement int dup(int oldfd) -> returns new dup-ed file descriptor on succsess or -1 on error


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
