#include "comm.h"
#include "echo.h"

#define NOT_EXIST -1

int main()
{
    int maxFd = -1;
    int listenFd = Socket(AF_INET, SOCK_STREAM, 0);
    maxFd = listenFd > maxFd ? listenFd : maxFd;

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(ECHO_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    Listen(listenFd, MAX_LISTEN_CON);

    fd_set allSet;// 真正需要检查的fd集合
    FD_ZERO(&allSet);
    FD_SET(listenFd, &allSet);

    int clinet[MAX_CONNECTION]; // 存储fd
    int maxindex = -1;
    for (int i = 0; i < MAX_CONNECTION; i++)
    {
        clinet[i] = NOT_EXIST;
    }
    
    while (1)
    {
        fd_set readSet = allSet; //获取一个fdset的备份，在select会被改
        // 记录已经被读的fd个数
        int ready = Select(maxFd + 1, &readSet, NULL, NULL, NULL);
        // 有客户端连接
        if (FD_ISSET(listenFd, &readSet))
        {
            int acceptFd = Accept(listenFd, NULL, NULL);
            // 找一个位置存放和clinet通信的fd
            int i = 0;
            for (; i < MAX_CONNECTION; i++)
            {
                if (clinet[i] == NOT_EXIST)
                {
                    clinet[i] = acceptFd;
                    maxindex = i > maxindex ? i : maxindex; //更新大的index
                    break;
                }
            }
            if (i == MAX_CONNECTION) //巧妙地判断上一个for循环有无命中
            {
                err_quit("too many clinet connections");
            }
            //只有找到了位置存储，才真正获取了一个可以服务的fd，才进行下面的步骤
            maxFd = acceptFd > maxFd ? acceptFd : maxFd; //更新最大的fd
            FD_SET(acceptFd, &allSet);
            ready--;
        }
        
        for (int i = 0; i <= maxindex && ready > 0; i++)
        {
            // 查找到某个客户端的acceptFd可读
            if (clinet[i] != NOT_EXIST && FD_ISSET(clinet[i], &readSet))
            {
                // 客户端已经关闭连接
                if (ServerProc(clinet[i]) == 0)
                {
                    Close(clinet[i]);
                    FD_CLR(clinet[i], &allSet);
                    clinet[i] = NOT_EXIST;
                }
                ready--;
            }
        }
        
    }
    return 0;
}