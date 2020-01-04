#include "comm.h"

static void alarmProc(int signo)
{
    return;
}

int main(int argc, char**argv)
{
    

    //创建连接fd
    int conFd = Socket(AF_INET, SOCK_STREAM, 0);

    //填写服务器信息
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    Inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);
    serverAddr.sin_port = htons(atoi(argv[2]));

    // 5秒后发送SIGALRM信号，信号处理函数返回的时候会中断connnect
    Signal(SIGALRM, alarmProc);
    alarm(5);

    int n;
    if ((n = connect(conFd, (SA *)&serverAddr, sizeof(serverAddr))) < 0)
    {
        close(conFd);
        if (errno == EINTR)
        {
            err_sys("connect timeout");
        }
        else
        {
            err_sys("connect error");
        }
    }
    

    char buf[MAX_BUF_SIZE];

    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(conFd, &rset);
    // select超时选项
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    // 利用select 最后一个参数超时返回
    n = select(conFd + 1, &rset, NULL, NULL, &tv);
    if (n == 0)
    {
        printf("read timeout\n");
    }
    else
    {
        read(conFd, buf, MAX_BUF_SIZE);
    }

    // 所有读操作都设置超时
    Setsockopt(conFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    // 所有写操作都设置超时
    Setsockopt(conFd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    

    return n;
}