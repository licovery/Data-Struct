#include "comm.h"


void ClientProc1(int sockFd, struct sockaddr *serverAddr, socklen_t addrLen)
{
    int inputFd = fileno(stdin);
    int outputFd = fileno(stdout);

    char buf[MAX_BUF_SIZE];
    int n;
    while ((n = Read(inputFd, buf, MAX_BUF_SIZE)) != 0)
    {
        // 第一次sendto内核会自动绑定客户端的IP和端口，可能会不断改变，对于多网卡的客户端
        Sendto(sockFd, buf, n, 0, serverAddr, addrLen);
        // 接收任意IP和端口来的报文，不一定是来着服务端，有安全问题
        Recvfrom(sockFd, buf, MAX_BUF_SIZE, 0, NULL, NULL);

        Writen(outputFd, buf, n);
    }
}

// connect设定udp连接，会返回错误，记录对端的IP和端口号，sendto和recvfrom不需要填addr
// 只要来自connect对端的udp报文才能进入缓冲区
void ClientProc2(int sockFd, struct sockaddr *serverAddr, socklen_t addrLen)
{
    // 设定对端地址的同时，也由内核绑定了本端IP和端口
    Connect(sockFd, serverAddr, addrLen);

    int inputFd = fileno(stdin);
    int outputFd = fileno(stdout);

    char buf[MAX_BUF_SIZE];
    int n;
    while ((n = Read(inputFd, buf, MAX_BUF_SIZE)) != 0)
    {
        // 已经绑定对端的IP和端口
        // 使用connect后，由已连接套接字引发的异步错误会返回给所在进程
        // 例如服务器已经退出，那么write后（write本身调用成功）的时候就会返回一个icmp不可达错误
        // 下次read调用就会出错，出错信息Connection refused
        Writen(sockFd, buf, n);
        n = Readn(sockFd, buf, n);

        Writen(outputFd, buf, n);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 0;
    }

    int sockFd = Socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    Inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);
    serverAddr.sin_port = htons(atoi(argv[2]));

    ClientProc2(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    
    return 0;
}