#include "comm.h"

void ServerProc(int acceptFd)
{
    int n;
    char buf[MAX_BUF_SIZE];
    while ((n = Read(acceptFd, buf, MAX_BUF_SIZE)) > 0)
    {
        Writen(acceptFd, buf, n);
    }
    Close(acceptFd);
}

// 处理结束的子进程
void HandleSigchild(int sig)
{
    int pid;
    int stat;
    // 因为信号不排队，必须考虑多个信号一起到来的情况，所以使用循环处理
    // 不用封装函数Waitpid，用了会直接异常退出
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        // IO不安全
        printf("child process %d terminated\n", pid);
    }
}



int main()
{
    int listenFd = Socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(ECHO_PORT);

    const int flag = 1;
    Setsockopt(listenFd, IPPROTO_TCP, SO_REUSEADDR, &flag, sizeof(flag));

    Bind(listenFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    Listen(listenFd, MAX_LISTEN_CON);

    Signal(SIGCHLD, HandleSigchild);

    while (1)
    {
        // accept可能会被信号处理函数返回的时候打断
        int acceptFd = accept(listenFd, NULL, NULL);
        if (acceptFd < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                err_sys("accept error");
            }
            
        }
        int pid = Fork();
        if (pid == 0)
        {
            // 子进程关闭listenFd
            Close(listenFd);
            ServerProc(acceptFd);
            exit(0);// 因为这里exit，子进程的作用范围只到这里，考虑处理僵尸进程
        }
        //父进程关闭acceptFd
        Close(acceptFd);
    }
    return 0;
}