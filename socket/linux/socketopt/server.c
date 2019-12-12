#include "comm.h"


int main()
{
    int listenFd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);

    Bind(listenFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    Listen(listenFd, MAX_LISTEN_CON);

    int connFd = Accept(listenFd, NULL, NULL);
    Close(listenFd);

    sleep(10);
    char buf[MAX_BUF_SIZE];
    while (Read(connFd, buf, MAX_BUF_SIZE) > 0);

    Close(connFd);
    return 0;
}