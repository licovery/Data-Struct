#include "comm.h"

#define NOT_CARE -1

int main()
{
    int listenFd = Socket(AF_INET, SOCK_STREAM, 0);
   
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(ECHO_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    Listen(listenFd, MAX_LISTEN_CON);
    
    int maxIndex = -1;
    struct pollfd clinet[MAX_CONNECTION];
    for (int i = 0; i < MAX_CONNECTION; i++)
    {
        clinet[i].fd = NOT_CARE;
    }

    clinet[0].fd = listenFd;
    clinet[0].events = POLLRDNORM;
    maxIndex = 0;

    char buf[MAX_BUF_SIZE];

    while (1)
    {
        // 超时-1，永远阻塞
        int ready = Poll(clinet, maxIndex + 1, -1);
        // listenFd有普通读数据
        if (clinet[0].revents & POLLRDNORM)
        {
            int connFd = Accept(listenFd, NULL, NULL);
            int i;
            for(i = 0; i < MAX_CONNECTION; i++)
            {
                if (clinet[i].fd < 0)
                {
                    clinet[i].fd = connFd;
                    clinet[i].events = POLLRDNORM;
                    maxIndex = i > maxIndex ? i : maxIndex;
                    break;
                }
            }
            if (i == MAX_CONNECTION)
            {
                err_quit("too many client connections");
            }
            ready--;
        }
        for (int i = 0; i < MAX_CONNECTION && ready > 0; i++)
        {
            // 找到可读的connFd
            if ((clinet[i].fd != NOT_CARE) && (clinet[i].revents & POLLRDNORM))
            {
                int n = Read(clinet[i].fd, buf, MAX_BUF_SIZE);
                // 客户端断开连接
                if (n == 0)
                {
                    Close(clinet[i].fd);
                    clinet[i].fd = -1;
                }
                else
                {
                    Writen(clinet[i].fd, buf, n);
                }
                ready--;
            }
        }
    }
    return 0;
}