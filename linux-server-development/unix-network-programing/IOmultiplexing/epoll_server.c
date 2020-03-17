#include "comm.h"
#include "echo.h"


int main()
{
    int listenFd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(ECHO_PORT);

    Bind(listenFd, (SA *)&serverAddr, sizeof(serverAddr));
    Listen(listenFd, MAX_LISTEN_CON);

    int epfd = Epoll_create(MAX_CONNECTION);
    epoll_add_event(epfd, listenFd, EPOLLIN);
    struct epoll_event events[MAX_CONNECTION];
    while (1)
    {
        int nReady = Epoll_wait(epfd, events, MAX_CONNECTION, -1); 
        for (int i = 0; i < nReady; i++)
        {
            int curFd = events[i].data.fd;
            if (curFd == listenFd) //监听fd就绪
            {
                int connFd = Accept(listenFd, NULL, NULL);
                epoll_add_event(epfd, connFd, EPOLLIN);
            }
            else // 已连接fd就绪
            {
                if (ServerProc(curFd) == 0)
                {
                    epoll_delete_event(epfd, curFd);
                    Close(curFd);
                }
            }
        }
    }
    return 0;
}