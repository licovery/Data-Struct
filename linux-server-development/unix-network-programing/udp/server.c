#include "comm.h"

void ServerProc(int sockFd)
{
    struct sockaddr_in clientAddr = {0};
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buf[MAX_BUF_SIZE];
    while (1)
    {
        // 直接recvfrom 后面两个参数是客户端地址以及地址长度。
        int n = Recvfrom(sockFd, buf, MAX_BUF_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
        // 发送nByte回给客户端，源IP端口由bind指定，目的Ip和端口取决与从recvfrom获取的clientAddr
        Sendto(sockFd, buf, n, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    }
}

int main()
{
    // SOCK_DGRAM是udp
    int sockFd = Socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(ECHO_PORT);

    // 服务器绑定udp端口
    Bind(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    ServerProc(sockFd);
    
    return 0;
}