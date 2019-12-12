#include "comm.h"

void Usage(char *arg0)
{
    printf("./%s <ip> <port>\n", arg0);
}



int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 0;
    }
    int maxFd = -1;
    int conFd = Socket(AF_INET, SOCK_STREAM, 0);
    maxFd = conFd > maxFd ? conFd : maxFd;

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    Inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);
    serverAddr.sin_port = htons(atoi(argv[2]));

    Connect(conFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    int stdinFd = fileno(stdin);
    maxFd = stdinFd > maxFd ? stdinFd : maxFd;

    fd_set allset;
    FD_ZERO(&allset);
    FD_SET(stdinFd, &allset);
    FD_SET(conFd, &allset);

    // 是否客户端已经申请关闭
    int sendShutDown = 0;

    while (1)
    {
        fd_set readset = allset;
        Select(maxFd + 1, &readset, NULL, NULL, NULL);
        
        char buf[MAX_BUF_SIZE] = {0};
        
        // 如果stdinFd可读
        if (FD_ISSET(stdinFd, &readset))
        {
            int n = Read(stdinFd, buf, MAX_BUF_SIZE);
            // 输入结束
            if (n == 0)
            {
                FD_CLR(stdinFd, &allset);
                Close(stdinFd);
                // half close 半关闭,通知服务不会再有新的请求发送过去。
                Shutdown(conFd, SHUT_WR);
                sendShutDown = 1;
            }
            else
            {
                Writen(conFd, buf, MAX_BUF_SIZE);
            }
        }

        // 如果conFd可读
        if (FD_ISSET(conFd, &readset))
        {
            int n = Read(conFd, buf, MAX_BUF_SIZE);
            // 服务器关闭,在这里最后再判断一下倒是是服务器提前关闭还是我发了shutdown之后再关闭的。
            if (n == 0)
            {
                if (sendShutDown)
                {
                    Close(conFd);
                    break;
                }
                else
                {
                    err_quit("server terminate too early");
                }
                
            }
            Writen(fileno(stdout), buf, n);
        }
    }
    return 0;
}