#include "comm.h"
#include <stdarg.h>



void CheckStatus(int status)
{
    if (status < 0)
    {
        perror("status error");
        exit(-1); 
    }
}

void CheckPointer(void *ptr)
{
    if (ptr == NULL)
    {
        perror("ptr is NULL");
        exit(-1);
    }
}

void ShowSockAddr(const struct sockaddr_in *addr)
{
    if (addr == NULL)
    {
        fprintf(stderr, "%s: ptr is NULL\n", __FUNCTION__);
        return;
    }
    //ipV4 str地址长度
    char ipStr[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &addr->sin_addr, ipStr, sizeof(ipStr));
    uint16_t port = ntohs(addr->sin_port);
    printf("%s:%d\n", ipStr, port);
}

void ShowAddrInfo(struct addrinfo *result)
{
    if (result == NULL)
    {
        fprintf(stderr, "%s: ptr is NULL\n", __FUNCTION__);
        return;
    }

    char ipStr[IPV6_STR_ADDR_LEN];

    for (struct addrinfo *ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        printf("ai_flags: %d\n", ptr->ai_flags);
        printf("ai_family: %d\n", ptr->ai_family);
        printf("ai_socktype: %d\n", ptr->ai_socktype);
        printf("ai_protocol: %d\n", ptr->ai_protocol);

        printf("cname: %s\n", ptr->ai_canonname);

        printf("addrlen: %d\n", ptr->ai_addrlen);

        int port;
        if (ptr->ai_family == AF_INET)
        {
            struct sockaddr_in *addr = (struct sockaddr_in *)(ptr->ai_addr);
            Inet_ntop(AF_INET, &addr->sin_addr, ipStr, sizeof(ipStr));
            port = ntohs(addr->sin_port);
        }
        else if (ptr->ai_family == AF_INET6)
        {
            struct sockaddr_in6 *addr = (struct sockaddr_in6 *)(ptr->ai_addr);
            Inet_ntop(AF_INET6, &addr->sin6_addr, ipStr, sizeof(ipStr));
            port = ntohs(addr->sin6_port);
        }
        else
        {
            fprintf(stderr, "unknow family\n");
            continue;
        }
        
        printf("addr: %s:%d\n", ipStr, port);
        printf("===============================================\n");
    }
}

void err_sys(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
    char	buf[MAXLINE];
    vsnprintf(buf, MAXLINE, fmt, ap);
    perror(buf);
	va_end(ap);
	exit(-1);
}

void err_quit(const char *fmt, ...)
{
    va_list		ap;
	va_start(ap, fmt);
    char	buf[MAXLINE];
    vsnprintf(buf, MAXLINE, fmt, ap);
    fprintf(stderr, "%s\n", buf);
    va_end(ap);
	exit(-1);
}

int TcpConnect(char *ip, short port)
{
    //创建连接fd
    int conFd = Socket(AF_INET, SOCK_STREAM, 0);

    //填写服务器信息
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    Inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(port);

    //建立tcp连接
    Connect(conFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("connect to ");
    ShowSockAddr(&serverAddr);

    return conFd;
}

int TcpListen(short port)
{
    //创建监听fd
    int listenFd = Socket(AF_INET, SOCK_STREAM, 0);

    int flag = 1;
    Setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    
    //填入地址端口信息
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);
    
    //fd绑定地址端口信息
    Bind(listenFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    printf("bind on ");
    ShowSockAddr(&serverAddr);

    //监听fd
    Listen(listenFd, MAX_LISTEN_CON);

    return listenFd;
}

int UdpConnect(char *ip, short port)
{
    //创建连接fd
    int conFd = Socket(AF_INET, SOCK_DGRAM, 0);

    //填写服务器信息
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    Inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(port);

    //绑定远程地址，并没有真正connect
    Connect(conFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("connect to ");
    ShowSockAddr(&serverAddr);
    
    return conFd;
}

int UdpBind( short port)
{
    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    int flag = 1;
    Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    
    //填入地址端口信息
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    Bind(sockfd, (SA *)&serverAddr, sizeof(serverAddr));
}

void Usage(char *path)
{
    printf("./%s <serverIp> <port>\n", path);
}

