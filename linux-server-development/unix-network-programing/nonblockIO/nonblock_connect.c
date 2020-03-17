#include "comm.h"

char buf[MAX_BUF_SIZE];

int SetNonBlockIo(int fd)
{
    int flag = Fcntl(fd, F_GETFL, 0);
    Fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    return flag; // 未设置blockIo的之前的状态，以便恢复
}

void NonblockConnect(int fd, SA *addr, socklen_t addrLen, int second)
{
    int originFlag = SetNonBlockIo(fd);

    fd_set rset, wset;
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_SET(fd, &rset);
    FD_SET(fd, &wset);
    int maxfd = fd + 1;
    struct timeval tv;
    tv.tv_sec = second;
    tv.tv_usec = 0;

    int n; ;
    // 可以for循环同时调用多个connect，非阻塞下同时connect加快处理速度
    if ((n = connect(fd, addr, addrLen)) < 0)
    {
        if (errno != EINPROGRESS) // 注意这里不是EWOULDBLOCK
        {
            err_sys("connect error");
        }
        // 连接成功，fd可写，连接fd既可写又可读，可能连接成功并且对端已经发来数据。所以不能根据fd只可写不可读来判断connect成功
        n = Select(maxfd, &rset, &wset, NULL, &tv); // 这里阻塞了
        if (n == 0)
        {
            err_quit("connect timeout");
        }
        if (FD_ISSET(fd, &rset) || FD_ISSET(fd, &wset))
        {
            int  error;
            socklen_t len = sizeof(error);
            Getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len); //判断连接的状态
            if (error == 0)
            {
                printf("nonblock connect succ");
            }
            else
            {
                err_sys("nonblock connect fail");
            }
        }
    }
    else
    {
        printf("succ with connect return 0");
    }
    

    Fcntl(fd, F_SETFL, originFlag);
} 

int main(int argc, char *argv[])
{
    struct addrinfo *ai = Host_serv(argv[1], argv[2], AF_UNSPEC, SOCK_STREAM);
    int timeout = 3;
    int fd = Socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    NonblockConnect(fd, ai->ai_addr, ai->ai_addrlen, timeout);
    sleep(5);

    // 当缓冲区有数据的时候，close会导致发送RST而不是FIN，如果实在不需处理读缓冲区，那么使用shutdown而不是close
    Read(fd, buf, MAX_BUF_SIZE);

    Close(fd);
    freeaddrinfo(ai);
}
