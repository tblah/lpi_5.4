#include <stdio.h>		// for printf and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit
#include <unistd.h>		// for fcntl
#include <fcntl.h>		// for fcntl

// implement int dup(int oldfd) -> returns new dup-ed file descriptor on succsess or -1 on error


void errExit(char* errMsg)
{
	int errorno = errno;

	printf("Exiting on error:\t%s\n", errMsg);
	printf("\n\nThe errno was %i\n", errorno);
	
	fflush(stdout);

	exit(EXIT_FAILURE);
}

int dup(int oldfd) {
	return fcntl(oldfd, F_DUPFD, 0);	// duplicate oldfd to the first available fd number greater than or equal to 0
}

int main(int argc, char* argv[])
{
	// test dup

	exit(EXIT_SUCCESS);
}
