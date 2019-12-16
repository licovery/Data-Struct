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
    printf("socket adrr: %s:%d\n", ipStr, port);
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

