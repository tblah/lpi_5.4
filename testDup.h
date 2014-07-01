#ifndef TEST_DUP_H
#define TEST_DUP_H

typedef enum {false, true} bool;

void errExit(char* errMsg);
int openTemporaryFile();
bool testDup(int fd1, int fd2);


#endif