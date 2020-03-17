#include "comm.h"


static void *ServerProc(void *arg)
{
    if (arg == NULL)
    {
        return NULL;
    }

    Pthread_detach(Pthread_self());

    int sockfd = *((int *)arg);
    free(arg); // 释放参数的内存；

    char buf[MAX_BUF_SIZE];
    int n;

    while ((n = Read(sockfd, buf, MAX_BUF_SIZE)) > 0)
    {
        Writen(sockfd, buf, n);
    }

    Close(sockfd);

    return NULL;
}

int main()
{
    pthread_t tid;
    int listenFd = TcpListen(ECHO_PORT);
    while (1)
    {
        int *psockfd = (int *)malloc(sizeof(int));
        *psockfd = Accept(listenFd, NULL, NULL);
        // 传参数要保证子线程间隔离，不能是指向局部变量的指针
        Pthread_create(&tid, NULL, ServerProc, psockfd);
    }
    return 0;
}