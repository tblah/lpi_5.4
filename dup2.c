#include <stdio.h>		// for printf and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit
#include "testDup.h"	// errExit and testDup
#include <unistd.h>		// for fcntl and close
#include <fcntl.h>		// for fcntl

// implement int dup2(int oldfd, int newfd) -> returns new dup-ed fd on success or -1 on error
// makes duplicate of oldfd to an fd = newfd. 
// if newfd is already open then this is closed by dup2 (ignore errors silently)
// if oldfd is invalid then fail with errno = EBADF and newfd is not closed
// if oldfd is valid and oldfd = newfd then do nothing and return oldfd. 

int dup2(int oldfd, int newfd) {
	printf("Running my dup2()\n");
	// test to see if oldfd is invalid. If so fail.
	if (fcntl(oldfd, F_GETFL) == -1) {
		errno = EBADF;
		return -1;
	}

	// if oldfd == newfd do nothing
	if (oldfd == newfd)
		return oldfd;

	// if newfd is open then close it
	if (fcntl(newfd, F_GETFL) != -1)
		close(newfd);	// deliberately don't check for errors here because POSIX says so

	return fcntl(oldfd, F_DUPFD, newfd);	// duplicate oldfd to the first available fd number greater than or equal to newfd
}

int main(int argc, char* argv[])
{
	// create two fd: fd1 being a temporary file and fd2 being a dup of fd1
	int fd1 = openTemporaryFile();
	int fd2 = dup2(fd1, 100);
	if (fd2 == -1)
		errExit("dup2");

	if (testDup(fd1, fd2) == false) {			// it doesnt matter that dup2 has two arguments
		printf("Dup failed tests\n");
		exit(EXIT_FAILURE);
	}
	else
		printf("Dup2 passed tests\n");

	if (close(fd1) == -1)	// explicit close is not necessary but good practice
		errExit("close");

	exit(EXIT_SUCCESS);
}
