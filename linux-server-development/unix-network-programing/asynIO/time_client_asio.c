#include "comm.h"
#include <string.h>

int done = 0;

void ProcRsp(sigval_t value)
{
    struct aiocb *aiocbp = (struct aiocb *)(value.sival_ptr);
    int n  = Aio_return(aiocbp);
    printf("\n rsp len:%d\n", n);
    volatile char *buf = aiocbp->aio_buf;
    for (int i = 0; i < n; i++)
    {
        printf("%c", buf[i]);
    }
    printf("\n");
    done = 1;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 0;
    }

    char *ip = argv[1];
    short port = atoi(argv[2]);

    int sockfd = TcpConnect(ip, port);

    char buf[MAX_BUF_SIZE] = {0};

    char *req = "hello world";
    printf("send msg: %s\n", req);
    Writen(sockfd, req, strlen(req) + 1);

    struct aiocb asynReadcb = {0};
    asynReadcb.aio_fildes = sockfd;
    asynReadcb.aio_buf = buf; //这里没有动态申请内存,读写共享一个缓冲区
    asynReadcb.aio_nbytes = MAX_BUF_SIZE;
    
    // 使用回调函数进行异步通知
    asynReadcb.aio_sigevent.sigev_notify = SIGEV_THREAD;
    asynReadcb.aio_sigevent.sigev_notify_attributes = NULL;
    asynReadcb.aio_sigevent.sigev_notify_function = ProcRsp;
    // 记录回调上下文
    asynReadcb.aio_sigevent.sigev_value.sival_ptr = &asynReadcb;

    Aio_read(&asynReadcb);

    while (!done)
    {
        printf("."); //证明是异步，读消息过程中还可以处理其他事情（例如打印）
        fflush(stdout);
        sleep(1);
    }
    
    Close(sockfd);
    return 0;
}