#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include "comm.h"
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>

int main(int argc, char *argv[])
{
    int status = 0;
    
    //创建监听fd
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    checkStatus(listenFd);
    
    //填入地址端口信息
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(50000);
    
    //fd绑定地址端口信息
    status = bind(listenFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    checkStatus(status);
    printf("bind on port: %d\n", ntohs(serverAddr.sin_port));
    
    //监听fd
    status = listen(listenFd, MAX_LISTEN_CON);
    checkStatus(status);

    while(1)//不退出
    {
        struct sockaddr_in clientAddr = {0};
        int addrLen = sizeof(clientAddr);
        
        //接受客户端连接，获取客户端地址端口信息（调试）
        int acceptFd = accept(listenFd, (struct sockaddr *)&clientAddr, &addrLen);
        checkStatus(listenFd);
        
        //输出客户端信息
        char clinetIp[20] = {0};
        inet_ntop(AF_INET, &clientAddr.sin_addr, clinetIp, 20);
        printf("connect from clinet %s:%d\n", clinetIp, ntohs(clientAddr.sin_port));
        
        //设置发送不缓存，暂时没有生效
        int flag = 1;
        status = setsockopt(acceptFd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
        checkStatus(status);

        //获取时间并计算rsp长度
        time_t t = time(NULL);
        char buf[MAX_BUF_SIZE] = {0};
        int msgSize = strlen(ctime(&t)) + 1;
        printf("message size:%d\n", msgSize);

        //先发送rsp长度，通知客户端下一次发送数据的大小
        *(int *)buf = htonl(msgSize);
        status = send(acceptFd, buf, sizeof(int), 0);
        checkStatus(status);
        
        memset(buf,0,MAX_BUF_SIZE);
        snprintf(buf, MAX_BUF_SIZE, "%s", ctime(&t));
        char *pBuf = buf;
        //一个个字符地发送，可能实际会缓存起来再一次性发送
        for(int i = 0; i < msgSize; i++)
        {
            status = send(acceptFd, pBuf + i, 1, 0);
            checkStatus(status);
            if (status == 0)
            {
                perror("send nothing\n");
            }
        }

        // close(acceptFd); send成功只是把数据写到内核缓冲区，不一定已经发送到对端
        printf("send complete\n");
    }

    return 0;
}