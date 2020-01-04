#include "comm.h"

int main()
{
    int connFd = Socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    Inet_pton(AF_INET, "192.168.11.11", &serverAddr.sin_addr); //不存在的地址，connect超时大约需要75秒
    serverAddr.sin_port = htons(ECHO_PORT);
    

    struct timeval opt;
    opt.tv_sec = 2;
    opt.tv_usec = 0;
    // 对于Linux，SO_SNDTIMEO选项对于connect有效,Unix则不一定
    Setsockopt(connFd, SOL_SOCKET, SO_SNDTIMEO, &opt, sizeof(opt));

    int n = connect(connFd, (SA *)&serverAddr, sizeof(serverAddr));
    if (n == -1)
    {
        if (errno == EINPROGRESS)
        {
            perror("connect timeout");
        }
    }

    return 0;
}