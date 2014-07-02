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

int dup2(int oldfd, int newfd) {	// fails silently if newfd is invalid
	//printf("Running my dup2()\n");
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

bool testDup2() {
	bool ret;

	// create two fd: fd1 being a temporary file and fd2 being a dup of fd1
	int fd1 = openTemporaryFile();

	int fd2 = dup2(fd1, 100);
	if (fd2 == -1)
		errExit("dup2");

	// tests for dup apply
	ret = testDup(fd1, fd2);

	// if oldfd == newfd then should do nothing
	if (dup2(fd1, fd1) == fd1)
		printf("Test 2.0: oldfd == newfd \t\t\t\t\t PASS\n");
	else {
		printf("Test 2.0: oldfd == newfd \t\t\t\t\t FAIL\n");
		ret = false;
	}

	// fail if oldfd is invalid
	int res = dup2(-1, -2);
	if ((res == -1) && (errno == EBADF))
		printf("Test 2.1: oldfd is invalid: \t\t\t\t\t PASS\n");
	else {
		printf("Test 2.1: oldfd is invalid: \t\t\t\t\t FAIL\n");
		ret = false;
	}

	// if newfd is open then close it
	int fd3 = openTemporaryFile();

	if (dup2(fd3, fd1) == -1) {	// this should close fd1 and so also close fd2
		printf("Test 2.2: newfd is open \t\t\t\t\t FAIL\n");
		ret = false;
	}
	else
		printf("Test 2.2: newfd is open \t\t\t\t\t PASS\n");

	// newfd is the requested fd
	if (dup2(fd1, 100) == 100)
		printf("Test 2.3: newfd is the requested fd \t\t\t\t PASS\n");
	else {
		printf("Test 2.3: newfd is the requested fd \t\t\t\t FAIL\n");
		ret = false;
	}

	if (close(fd1) == -1)	// explicit close is not necessary but good practice
		errExit("close");

	return ret;
}

int main(int argc, char* argv[])
{
	if (testDup2() == false) {
		printf("Dup failed tests\n");
		exit(EXIT_FAILURE);
	}
	else
		printf("Dup2 passed tests\n");

	exit(EXIT_SUCCESS);
}
