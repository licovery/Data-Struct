#include "comm.h"

int main()
{
    int connFd = Socket(AF_INET, SOCK_STREAM, 0);
    
    struct linger opt = {0};
    opt.l_onoff = 0;
    opt.l_linger = 0;
    Setsockopt(connFd, SOL_SOCKET, SO_LINGER, &opt, sizeof(opt));
    
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    Inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
    servAddr.sin_port = htons(SERVER_PORT);
    
    Connect(connFd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    
    char buf[1024000];
    write(connFd, buf, 1024000);
    
    if (close(connFd) < 0 && errno == EWOULDBLOCK)
    {
        printf("EWOULDBLOCK\n");
    }
    return 0;
}