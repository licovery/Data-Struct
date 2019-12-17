#include "comm.h"
#include "echo.h"

// 返回0代表客户端已经准备关闭
int ServerProc(int fd)
{
    char buf[MAX_BUF_SIZE];
    int n = 0;
    if ((n = Read(fd, buf, MAX_BUF_SIZE)) > 0)
    {
        Writen(fd, buf, n);
    }
    return n;
}