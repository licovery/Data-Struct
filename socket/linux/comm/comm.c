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

void ShowSockaddr(const struct sockaddr_in *addr)
{
    if (addr == NULL)
    {
        return;
    }
    //ipV4 str地址长度
    char ipStr[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &addr->sin_addr, ipStr, sizeof(ipStr));
    uint16_t port = ntohs(addr->sin_port);
    printf("socket adrr: %s:%d\n", ipStr, port);
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

