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

int openTemporaryFile() {
	FILE *tmp = tmpfile();	// create a temporary file for us to play with
	bool ret = true;

	if (tmp == NULL)
		errExit("tmpfile");

	int fd = fileno(tmp);	// we need the file descriptor of the temporary file
	if (fd == -1)
		errExit("fileno");

	return fd;
}

bool testFileOffsetsEqual(int fd1, int fd2) {	// DRY for testDup
	off_t offset1 = lseek(fd1, 0, SEEK_CUR);	// get offset for fd1
	off_t offset2 = lseek(fd2, 0, SEEK_CUR);	// get offset for fd2

	if ((offset1 == -1) || (offset2 == -1))
		errExit("lseek: getting file offset");

	if (offset1 == offset2)
		return true;

	return false;
}

void fileSeek(int fd, off_t amount) {
	if (lseek(fd, amount, SEEK_CUR) == -1)
		errExit("lseek: seeking by one byte");

}

int fileGetFlags(int fd) {
	int ret = fcntl(fd, F_GETFL);

	if (ret == -1)
		errExit("fcntl: getting file status flags");

	return ret;
}

bool testFileStatusFlagsEqual(int fd1, int fd2) {
	int flags1 = fileGetFlags(fd1);	// get file status flags
	int flags2 = fileGetFlags(fd2);	// get file status flags

	if (flags1 == flags2)
		return true;

	return false;
}

bool testDup(int fd1, int fd2) {	// the dup-ing is done outside this function so that different dup functions can be tested
	// if they are dup-ed then they should share a file offset
	bool ret = true;

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
		printf("Test 2: are file offsets equal after seek in fd1? \t\t FAIL\n");
		ret = false;
	}

	fileSeek(fd2, -1); // seek backwards by one byte

	if (testFileOffsetsEqual(fd1, fd2) == true)
		printf("Test 3: are file offsets equal after seek in fd2? \t\t PASS\n");
	else {
		printf("Test 3: are file offsets equal after seek in fd2? \t\t FAIL\n");
		ret = false;
	}

	// if they are dup-ed they should also share file status flags

	if (testFileStatusFlagsEqual(fd1, fd2) == true)
		printf("Test 4: are file status flags the same? \t\t\t PASS\n");
	else {
		printf("Test 4: are file status flags the same? \t\t\t FAIL\n");
		ret = false;
	}

	int flags = fileGetFlags(fd1);
	flags |= O_APPEND;
	if (fcntl(fd2, F_SETFL, flags) == -1)
		errExit("fcntl: setting flags");

	if (testFileStatusFlagsEqual(fd1, fd2) == true)
		printf("Test 5: are file status flags the same after a change? \t\t PASS\n");
	else {
		printf("Test 5: are file status flags the same after a change? \t\t FAIL\n");
		ret = false;
	}

	if (close(fd1) == -1)
		errExit("close");

	return ret;
}