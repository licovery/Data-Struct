#include "comm.h"

int main()
{
    int listenFd = Socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(ECHO_PORT);
    
   
    Bind(listenFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    Listen(listenFd, MAX_LISTEN_CON);

    struct timeval opt;
    opt.tv_sec = 2;
    opt.tv_usec = 0;
    // 对于Linux，SO_RCVTIMEO选项对于accept有效，Unix不一定
    Setsockopt(listenFd, SOL_SOCKET, SO_RCVTIMEO, &opt, sizeof(opt));

    int n = accept(listenFd, NULL, NULL);
    if (n == -1)
    {
        if (errno == EWOULDBLOCK)
        {
            perror("accept timeout");
        }
    }

    return 0;
}