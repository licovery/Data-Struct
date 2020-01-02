#include	"comm.h"

void *
Calloc(size_t n, size_t size)
{
	void	*ptr;

	if ( (ptr = calloc(n, size)) == NULL)
		err_sys("calloc error");
	return(ptr);
}

void
Close(int fd)
{
	if (close(fd) == -1)
		err_sys("close error");
}

pid_t
Fork(void)
{
	pid_t	pid;

	if ( (pid = fork()) == -1)
		err_sys("fork error");
	return(pid);
}

void *
Malloc(size_t size)
{
	void	*ptr;

	if ( (ptr = malloc(size)) == NULL)
		err_sys("malloc error");
	return(ptr);
}

#include	<sys/mman.h>

void *
Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	void	*ptr;

	if ( (ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *) -1))
		err_sys("mmap error");
	return(ptr);
}

int
Open(const char *pathname, int oflag, mode_t mode)
{
	int		fd;

	if ( (fd = open(pathname, oflag, mode)) == -1)
		err_sys("open error for %s", pathname);
	return(fd);
}

void
Pipe(int *fds)
{
	if (pipe(fds) < 0)
		err_sys("pipe error");
}

ssize_t
Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = read(fd, ptr, nbytes)) == -1)
		err_sys("read error");
	return(n);
}

void
Sigaddset(sigset_t *set, int signo)
{
	if (sigaddset(set, signo) == -1)
		err_sys("sigaddset error");
}

void
Sigdelset(sigset_t *set, int signo)
{
	if (sigdelset(set, signo) == -1)
		err_sys("sigdelset error");
}

void
Sigemptyset(sigset_t *set)
{
	if (sigemptyset(set) == -1)
		err_sys("sigemptyset error");
}

void
Sigfillset(sigset_t *set)
{
	if (sigfillset(set) == -1)
		err_sys("sigfillset error");
}

int
Sigismember(const sigset_t *set, int signo)
{
	int		n;

	if ( (n = sigismember(set, signo)) == -1)
		err_sys("sigismember error");
	return(n);
}

void
Sigpending(sigset_t *set)
{
	if (sigpending(set) == -1)
		err_sys("sigpending error");
}

void
Sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
	if (sigprocmask(how, set, oset) == -1)
		err_sys("sigprocmask error");
}

pid_t
Wait(int *iptr)
{
	pid_t	pid;

	if ( (pid = wait(iptr)) == -1)
		err_sys("wait error");
	return(pid);
}

pid_t
Waitpid(pid_t pid, int *iptr, int options)
{
	pid_t	retpid;

	if ( (retpid = waitpid(pid, iptr, options)) == -1)
		err_sys("waitpid error");
	return(retpid);
}

void
Write(int fd, void *ptr, size_t nbytes)
{
	if (write(fd, ptr, nbytes) != nbytes)
		err_sys("write error");
}


Sigfunc * 
Signal(int signo, Sigfunc *func)
{
	struct sigaction act, oldact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM)
	{
		act.sa_flags |= SA_INTERRUPT;
	}
	else
	{
		act.sa_flags |= SA_RESTART;		//重启因为信号处理被中断的慢系统调用，例如accept,read
	}
	
	

	if (sigaction(signo, &act, &oldact) < 0)
	{
		err_sys("sigaction error");
	}
	return oldact.sa_handler;
}

int Fcntl(int fd, int cmd, int flag)
{
	int n;
	if ((n = fcntl(fd, cmd, flag)) < 0)
	{
		err_sys("fcntl error");
	}
	return n;
}

int Ioctl(int fd, int request, void *arg)
{
	int n;
	if ( (n = ioctl(fd, request, arg)) == -1)
		err_sys("ioctl error");
	return(n);	
}


char * gf_time()
{
	struct timeval	tv;
	time_t			t;
	static char		str[30];
	char			*ptr;

	if (gettimeofday(&tv, NULL) < 0)
		err_sys("gettimeofday error");

	t = tv.tv_sec;	/* POSIX says tv.tv_sec is time_t; some BSDs don't agree. */
	ptr = ctime(&t);
	strcpy(str, &ptr[11]);
		/* Fri Sep 13 00:00:00 1986\n\0 */
		/* 0123456789012345678901234 5  */
	snprintf(str+8, sizeof(str)-8, ".%06ld", tv.tv_usec);

	return(str);
}

pthread_t Pthread_create(pthread_t *tid, pthread_attr_t *arrt, Threadfunc *func, void *arg)
{
	errno = pthread_create(tid, arrt, func, arg);
	if (errno != 0)
	{
		err_sys("pthread_create error");
	}
	return *tid;
}

void Pthread_join(pthread_t tid, void **retval)
{
	errno = pthread_join(tid, retval);
	if (errno != 0)
	{
		err_sys("pthread_join error");
	}
}

pthread_t Pthread_self()
{
	return pthread_self();
}

void Pthread_detach(pthread_t tid)
{
	errno = pthread_detach(tid);
	if (errno != 0)
	{
		err_sys("pthread_death error");
	}
}

// retval不能指向调用者的局部变量
void Pthread_exit(void *retval)
{
	pthread_exit(retval);
}


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
ssize_t Aio_return(struct aiocb* aiocbp)
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








