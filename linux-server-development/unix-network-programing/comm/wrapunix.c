#include "comm.h"

void *
Calloc(size_t n, size_t size)
{
    void *ptr;

    if ((ptr = calloc(n, size)) == NULL)
        err_sys("calloc error");
    return (ptr);
}

void Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close error");
}

pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) == -1)
        err_sys("fork error");
    return (pid);
}

void *
Malloc(size_t size)
{
    void *ptr;

    if ((ptr = malloc(size)) == NULL)
        err_sys("malloc error");
    return (ptr);
}

void *
Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    void *ptr;

    if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == MAP_FAILED)
        err_sys("mmap error");
    return (ptr);
}

void Munmap(void *addr, size_t len)
{
    if (munmap(addr, len) == -1)
    {
        err_sys("munmap error");
    }
}

void Msync(void *addr, size_t len, int flag)
{
    if (msync(addr, len, flag) == -1)
    {
        err_sys("msync error");
    }
}

int Open(const char *pathname, int oflag, mode_t mode)
{
    int fd;

    if ((fd = open(pathname, oflag, mode)) == -1)
        err_sys("open error for %s", pathname);
    return (fd);
}

void Pipe(int *fds)
{
    if (pipe(fds) < 0)
        err_sys("pipe error");
}

void Dup2(int fd1, int fd2)
{
    if (dup2(fd1, fd2) == -1)
        err_sys("dup2 error");
}

ssize_t
Read(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;

    if ((n = read(fd, ptr, nbytes)) == -1)
        err_sys("read error");
    return (n);
}

void Sigaddset(sigset_t *set, int signo)
{
    if (sigaddset(set, signo) == -1)
        err_sys("sigaddset error");
}

void Sigdelset(sigset_t *set, int signo)
{
    if (sigdelset(set, signo) == -1)
        err_sys("sigdelset error");
}

void Sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) == -1)
        err_sys("sigemptyset error");
}

void Sigfillset(sigset_t *set)
{
    if (sigfillset(set) == -1)
        err_sys("sigfillset error");
}

int Sigismember(const sigset_t *set, int signo)
{
    int n;

    if ((n = sigismember(set, signo)) == -1)
        err_sys("sigismember error");
    return (n);
}

void Sigpending(sigset_t *set)
{
    if (sigpending(set) == -1)
        err_sys("sigpending error");
}

void Sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
    if (sigprocmask(how, set, oset) == -1)
        err_sys("sigprocmask error");
}

pid_t Wait(int *iptr)
{
    pid_t pid;

    if ((pid = wait(iptr)) == -1)
        err_sys("wait error");
    return (pid);
}

pid_t Waitpid(pid_t pid, int *iptr, int options)
{
    pid_t retpid;

    if ((retpid = waitpid(pid, iptr, options)) == -1)
        err_sys("waitpid error");
    return (retpid);
}

void Write(int fd, void *ptr, size_t nbytes)
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
        act.sa_flags |= SA_RESTART; //重启因为信号处理被中断的慢系统调用，例如accept,read
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
    if ((n = ioctl(fd, request, arg)) == -1)
        err_sys("ioctl error");
    return (n);
}

char *gf_time()
{
    struct timeval tv;
    time_t t;
    static char str[30];
    char *ptr;

    if (gettimeofday(&tv, NULL) < 0)
        err_sys("gettimeofday error");

    t = tv.tv_sec; /* POSIX says tv.tv_sec is time_t; some BSDs don't agree. */
    ptr = ctime(&t);
    strcpy(str, &ptr[11]);
    /* Fri Sep 13 00:00:00 1986\n\0 */
    /* 0123456789012345678901234 5  */
    snprintf(str + 8, sizeof(str) - 8, ".%06ld", tv.tv_usec);

    return (str);
}
