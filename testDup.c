#include <stdio.h>		// for printf, tmpfile, fileno and fflush
#include <errno.h>		// for errno
#include <stdlib.h>		// for exit
#include <unistd.h>		// for fcntl and lseek
#include <fcntl.h>		// for fcntl
#include <sys/types.h>	// for open
#include <sys/stat.h>	// for open
#include <fcntl.h>		// for open
#include "testDup.h"	// for bool

void errExit(char* errMsg)
{
	int errorno = errno;

	printf("Exiting on error:\t%s\n", errMsg);
	printf("\n\nThe errno was %i\n", errorno);
	
	fflush(stdout);

	exit(EXIT_FAILURE);
}

bool testFileOffsetsEqual(int fd1, int fd2) {	// DRY for testDup
	off_t offset1 = lseek(fd1, 0, SEEK_CUR);	// get offset for fd1
	off_t offset2 = lseek(fd2, 0, SEEK_CUR);	// get offset for fd2

	if (offset1 == -1 || offset2 == -1)
		errExit("lseek: getting file offset");

	if (offset1 == offset2)
		return true;

	return false;
}

void fileSeek(int fd, off_t amount) {
	if (lseek(fd, amount, SEEK_CUR) == -1)
		errExit("lseek: seeking by one byte");

}

bool testDup() {
	FILE *tmp = tmpfile();	// create a temporary file for us to play with
	bool ret = true;

	if (tmp == NULL)
		errExit("tmpfile");

	int fd1 = fileno(tmp);	// we need the file descriptor of the temporary file
	if (fd1 == -1)
		errExit("fileno");

	int fd2 = dup(fd1);
	if (fd2 == -1)
		errExit("dup");

	if (testFileOffsetsEqual(fd1, fd2) == true)
		printf("Test 1: are file offsets equal to begin with? \t\t\t PASS\n");
	else {
		printf("Test 1: are file offsets equal to begin with? \t\t\t FAIL\n");
		ret = false;
	}

	fileSeek(fd1, 5);	// seek by 5 bytes

	if (testFileOffsetsEqual(fd1, fd2) == true)
		printf("Test 2: are file offsets equal after seek in fd1? \t\t PASS\n");
	else {
		printf("Test 1: are file offsets equal after seek in fd1? \t\t FAIL\n");
		ret = false;
	}

	fileSeek(fd2, -1);

	if (testFileOffsetsEqual(fd1, fd2) == true)
		printf("Test 2: are file offsets equal after seek in fd2? \t\t PASS\n");
	else {
		printf("Test 1: are file offsets equal after seek in fd2? \t\t FAIL\n");
		ret = false;
	}

	return ret;
}