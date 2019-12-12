#include <comm.h>

void Usage(char *path)
{
    printf("./%s <serverIp> <port>\n", path);
}

void ClinetProc(int conFd)
{
    int recvByte = 0;
    char buf[MAX_BUF_SIZE];
    //先接收服务通知的真正消息的大小，只收4个字节
    recvByte = Readn(conFd, buf, sizeof(int));
    if (recvByte == 0)
    {
        perror("connection close by peer\n");
        exit(-1);//进程退出，不用close也可以，会自动close
    }
    int msgSize = ntohl(*(int *)buf);
    printf("message size:%d\n", msgSize);
    
    memset(buf, 0, MAX_BUF_SIZE);
    char *pBuf = buf;
    int curSize = 0;
    //循环接收真正的数据
    while (curSize < msgSize)
    {
        recvByte = Read(conFd, pBuf, MAX_BUF_SIZE - curSize);
        if (recvByte == 0)
        {
            perror("connection close by peer\n");
            exit(-1);
        }
        curSize += recvByte;
        pBuf += recvByte;
        printf("recvByte:%d, curSize:%d\n",recvByte, curSize);
    }
    printf("response from server: %s", buf);
}

int main(int argc, char**argv)
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 0;
    }

    //创建连接fd
    int conFd = Socket(AF_INET, SOCK_STREAM, 0);

    //填写服务器信息
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    Inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);
    serverAddr.sin_port = htons(atoi(argv[2]));
    
    //建立tcp连接
    Connect(conFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
 
    //connect后才能getsockname
    struct sockaddr_in myAddr;
    socklen_t myAddrLen = sizeof(myAddr);
    Getsockname(conFd, (struct sockaddr *)&myAddr, &myAddrLen);
    ShowSockaddr(&myAddr);

    ClinetProc(conFd);
    
    Close(conFd);

    return 0;
}