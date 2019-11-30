#include "comm.h"
#include <stdio.h>
#include <stdlib.h>

void Usage(char *path)
{
    printf("./%s <serverIp> <port>\n", path);
}

void checkStatus(int status)
{
    if (status < 0)
    {
        perror("error\n");
        exit(-1);
    }
}

void checkPointer(void *ptr)
{
    if (ptr == NULL)
    {
        perror("ptr is NULL\n");
        exit(-1);
    }
}