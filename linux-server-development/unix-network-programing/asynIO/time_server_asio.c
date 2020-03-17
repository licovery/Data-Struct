#include "comm.h"


void IoProc(sigval_t context)
{
    struct aiocb *aiocbp = (struct aiocb *)(context.sival_ptr);
    int sockfd = aiocbp->aio_fildes;
    
    
    if (aio_error(aiocbp) != 0) //IO失败
    {
        perror("aio_error");
        Aio_cancel(sockfd, NULL);
        Close(sockfd);
        free(aiocbp->aio_buf);
        free(aiocbp);
        return;
    }
    int n = Aio_return(aiocbp);
    if (n == 0) // 对端关闭
    {
        fprintf(stderr, "peer close! sockfd:%d\n", sockfd);
        Close(sockfd);
        free(aiocbp->aio_buf);
        free(aiocbp);
        return;
    }

    sleep(5); //客户端异步IO体现更加明显

    Writen(sockfd, aiocbp->aio_buf, n);
    
    Aio_read(aiocbp);//再次读
}

void AioProc(int sockfd)
{
    struct aiocb *asynReadcb = malloc(sizeof(struct aiocb));
    bzero(asynReadcb, sizeof(struct aiocb));
    asynReadcb->aio_fildes = sockfd;
    asynReadcb->aio_buf = malloc(MAX_BUF_SIZE); //读取成功后释放内存
    asynReadcb->aio_nbytes = MAX_BUF_SIZE;
    
    // 使用回调函数进行异步通知
    asynReadcb->aio_sigevent.sigev_notify = SIGEV_THREAD;
    asynReadcb->aio_sigevent.sigev_notify_attributes = NULL;
    asynReadcb->aio_sigevent.sigev_notify_function = IoProc;
    // 回调上下文
    asynReadcb->aio_sigevent.sigev_value.sival_ptr = asynReadcb;

    Aio_read(asynReadcb);
}

int main()
{
    int listenFd = TcpListen(ECHO_PORT);
    fprintf(stderr, "listenFd:%d\n", listenFd);

    while (1)
    {
        int connFd = Accept(listenFd, NULL, NULL);
        fprintf(stderr, "connFd:%d\n", connFd);
        AioProc(connFd);
    }
    
    return 0;
}