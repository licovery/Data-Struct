#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "comm.h"


int main(int argc, char**argv)
{
    if (argc != 3)
    {
        Usage(argv[0]);
    }

    int status = 0;
    int conFd = socket(AF_INET, SOCK_STREAM, 0);
    checkStatus(conFd);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    checkStatus(inet_pton(AF_INET, argv[1], &serverAddr.sin_addr));
    serverAddr.sin_port = htons(atoi(argv[2]));

    status = connect(conFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    checkStatus(status);

    int recvByte = 0;
    char buf[MAX_BUF_SIZE];
    recvByte = recv(conFd, buf, MAX_BUF_SIZE, 0);
    checkStatus(recvByte);
    int msgSize = ntohl(*(int *)buf);
    
    char *pBuf = buf;
    int curSize = 0;
    while (curSize < msgSize)
    {
        recvByte = recv(conFd, pBuf, MAX_BUF_SIZE - curSize, 0);
        checkStatus(recvByte);
        curSize += recvByte;
        pBuf += recvByte;
    }

    printf("response from server: %s", buf);
    return 0;
}