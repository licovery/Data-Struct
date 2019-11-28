#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include "comm.h"
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int status = 0;

    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    checkStatus(listenFd);

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(50000);
    
    status = bind(listenFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    checkStatus(status);
    printf("bind on port: %d\n", ntohs(serverAddr.sin_port));

    status = listen(listenFd, MAX_LISTEN_CON);
    checkStatus(status);

    
    while(1)//不退出
    {
        struct sockaddr_in clientAddr = {0};
        int addrLen = sizeof(clientAddr);
        int acceptFd = accept(listenFd, (struct sockaddr *)&clientAddr, &addrLen);
        checkStatus(listenFd);
        char clinetIp[20] = {0};
        inet_ntop(AF_INET, &clientAddr.sin_addr, clinetIp, 20);
        printf("connect from clinet %s:%d\n", clinetIp, ntohs(clientAddr.sin_port));
        
        time_t t = time(NULL);

        char buf[MAX_BUF_SIZE] = {0};
        int size = htonl(strlen(ctime(&t) + 1));
        *(int *)buf = size;
        status = send(acceptFd, buf, MAX_BUF_SIZE, sizeof(int));
        checkStatus(status);

        memset(buf,0,MAX_BUF_SIZE);
        snprintf(buf, MAX_BUF_SIZE, "%s", ctime(&t));
        status = send(acceptFd, buf, MAX_BUF_SIZE, 0);
        checkStatus(status);
        
        close(acceptFd);
        printf("end connection\n");
    }

    return 0;
}