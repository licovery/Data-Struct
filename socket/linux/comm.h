#ifndef COMM_H
#define COMM_H

#define MAX_LISTEN_CON 10
#define MAX_BUF_SIZE 256

void Usage(char *path);
void checkStatus(int status);
void checkPointer(void *ptr);

#endif