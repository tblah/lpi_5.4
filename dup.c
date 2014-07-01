#include <stdio.h>		// for printf, tmpfile, fileno and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit
#include <unistd.h>		// for fcntl and lseek
#include <fcntl.h>		// for fcntl
#include <sys/types.h>	// for open
#include <sys/stat.h>	// for open
#include <fcntl.h>		// for open
#include "testDup.h"	// errExit and testDup


// implement int dup(int oldfd) -> returns new dup-ed file descriptor on success or -1 on error

int dup(int oldfd) {
	printf("Running my dup()\n");
	return fcntl(oldfd, F_DUPFD, 0);	// duplicate oldfd to the first available fd number greater than or equal to 0
}

int main(int argc, char* argv[])
{
	// test dup
	if (testDup() == false) {
		printf("Dup failed tests\n");
		exit(EXIT_FAILURE);
	}
	else
		printf("Dup passed tests\n");

	exit(EXIT_SUCCESS);
}
