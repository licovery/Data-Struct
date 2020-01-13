#include "comm.h"

// 异步IO asyn

void Aio_read(struct aiocb *aiocbp)
{
    int n = aio_read(aiocbp);
    if (n < 0)
    {
        err_sys("aio_read error");
    }
}

void Aio_write(struct aiocb *aiocbp)
{
    int n = aio_write(aiocbp);
    if (n < 0)
    {
        err_sys("aio_write error");
    }
}

// read和write的返回值，已经读或者写的数据量
ssize_t Aio_return(struct aiocb *aiocbp)
{
    ssize_t n = aio_return(aiocbp);
    if (n < 0)
    {
        err_sys("aio_return error");
    }
    return n;
}

// EINPROGRESS说明请求尚未完成
// ECANCELLED说明请求被应用程序取消了
// 0代表IO成功
int Aio_error(struct aiocb *aiocbp)
{
    int n = aio_error(aiocbp);
    return n;
}

// 超时也会返回错误，阻塞多个异步IO，直到有完成或者超时或错误
// size用于指示cblist数组的长度，其中为NULl元素会被aio_suspend忽略
// 不设置超时则把timeout设置为NULL
int Aio_suspend(const struct aiocb *const cblist[], int size, const struct timespec *timeout)
{
    int n = aio_suspend(cblist, size, timeout);
    if (n < 0)
    {
        if (errno == EAGAIN)
        {
            perror("aio_suspend timeout");
            return -1; // 超时返回-1
        }
        else
        {
            err_sys("aio_suspend error");
        }
    }
    return 0;
}

// AIO_CANCELED 全部成功取消
// AIO_NOTCANCELED 部分取消 可以使用aio_error判断请求是否取消，如果请求已经取消，aio_error会返回-1,并置errno为ECANCELED
// AIO_ALLDONE 全部已经完成了
// 若aiocbp为NULL，则取消此fd的所有异步io请求
int Aio_cancel(int fd, struct aiocb *aiocbp)
{
    int n;
    n = aio_cancel(fd, aiocbp);
    if (n == -1)
    {
        err_sys("aio_cancel error");
    }
    return n;
}

int AioIsCancel(struct aiocb *aiocbp)
{
    return (aio_error(aiocbp) == -1) && (errno == ECANCELED);
}

// mode LIO_WAIT阻塞   LIO_NOWAIT马上返回
// sig 异步回调通知的方式
// 一次性发起多个IO，具体IO的方式由aio_lio_opcode决定(LIO_READ,LIO_WRITE,LIO_NOP)
void Lio_listio(int mode, struct aiocb *list[], int nent, struct sigevent *sig)
{
    int n = lio_listio(mode, list, nent, sig);
    if (n < 0)
    {
        err_sys("lio_listio error");
    }
}