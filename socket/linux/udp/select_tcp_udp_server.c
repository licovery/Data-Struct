#include "comm.h"

static char buf[MAX_BUF_SIZE];

static void ServerTcpProc(int connFd)
{
    int n;
    while ((n = Read(connFd, buf, MAX_BUF_SIZE)) > 0)
    {
        Writen(connFd, buf, n);
    }
    Close(connFd);
}

static void ServerUdpProc(int udpFd)
{
    struct sockaddr_in clientAddr;
    socklen_t addrLen =  sizeof(clientAddr);
    Recvfrom(udpFd, buf, MAX_BUF_SIZE, 0, (struct sockaddr *)&clientAddr, &addrLen);
    Sendto(udpFd, buf, MAX_BUF_SIZE, 0, (struct sockaddr *)&clientAddr, addrLen);
}

// 处理结束的子进程
static void HandleSigchild(int signo)
{
    int pid;
    // 因为信号不排队，必须考虑多个信号一起到来的情况，所以使用循环处理
    // 不用封装函数Waitpid，用了会直接异常退出
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0)
    {
        // IO不安全
        printf("child process %d terminated\n", pid);
    }
}



int main()
{

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);

    int maxFd = -1;

    // tcp
    int listenFd = Socket(AF_INET, SOCK_STREAM, 0);
    maxFd = listenFd > maxFd ? listenFd : maxFd;

    const int flag = 1;
    Setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    Bind(listenFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    Listen(listenFd, MAX_LISTEN_CON);

    // udp
    int udpFd = Socket(AF_INET, SOCK_DGRAM, 0);
    maxFd = udpFd > maxFd ? udpFd : maxFd;
    Bind(udpFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    fd_set rset, allSet;
    FD_ZERO(&rset);
    FD_ZERO(&allSet);
    FD_SET(listenFd, &allSet);
    FD_SET(udpFd, &allSet);

    Signal(SIGCHLD, HandleSigchild);

    while (1)
    {
        rset = allSet;
        // select可能会被信号处理函数返回的时候打断
        int nready = select(maxFd + 1, &rset, NULL, NULL, NULL);
        if (nready < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                err_sys("select error");
            }
        }

        if (FD_ISSET(listenFd, &rset))
        {
            int connFd = Accept(listenFd, NULL, NULL);
            if (Fork() == 0)
            {
                // 子进程关闭listenFd
                Close(listenFd);
                ServerTcpProc(connFd);
                exit(0);// 因为这里exit，子进程的作用范围只到这里，考虑处理僵尸进程
            }
            //父进程关闭acceptFd
            Close(connFd);
        }

        if (FD_ISSET(udpFd, &rset))
        {
            ServerUdpProc(udpFd);
        }
    }
    return 0;
}