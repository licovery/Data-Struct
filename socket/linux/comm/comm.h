#ifndef COMM_H
#define COMM_H

// linux(unix)内核
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <netinet/tcp.h>
#include <sys/stropts.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <aio.h>
#include <limits.h>
#include <sys/stat.h>

// standard C
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

// POSIX IPC
#include <mqueue.h>
#include <semaphore.h>
#include <sys/mman.h>

// 限制值
#define MAX_LISTEN_CON 16
#define MAX_BUF_SIZE 4096
#define MAXLINE 1024
#define MAX_CONNECTION 1024
#define IPV4_STR_ADDR_LEN 16
#define IPV6_STR_ADDR_LEN 64

// 端口
#define DAYTIME_PORT 30000
#define ECHO_PORT 30001
#define SERVER_PORT 9877

void CheckStatus(int status);
void CheckPointer(void *ptr);
void ShowSockAddr(const struct sockaddr_in *addr);
void ShowAddrInfo(struct addrinfo *result);

#define SA struct sockaddr
typedef void Sigfunc(int);        /* for signal handlers */
typedef void *Threadfunc(void *); /* for thread handlers */
#define bzero(ptr, n) memset(ptr, 0, n)
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// wapper函数

// socket
int Accept(int, SA *, socklen_t *);
void Bind(int, const SA *, socklen_t);
void Connect(int, const SA *, socklen_t);
void Getpeername(int, SA *, socklen_t *);
void Getsockname(int, SA *, socklen_t *);
void Getsockopt(int, int, int, void *, socklen_t *);
void Listen(int, int);
int Poll(struct pollfd *, unsigned long, int);
ssize_t Readline(int, void *, size_t);
ssize_t Readn(int, void *, size_t);
ssize_t Recv(int, void *, size_t, int);
ssize_t Recvfrom(int, void *, size_t, int, SA *, socklen_t *);
ssize_t Recvmsg(int, struct msghdr *, int);
int Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
void Send(int, const void *, size_t, int);
void Sendto(int, const void *, size_t, int, const SA *, socklen_t);
void Sendmsg(int, const struct msghdr *, int);
void Setsockopt(int, int, int, const void *, socklen_t);
void Shutdown(int, int);
int Sockatmark(int);
int Socket(int, int, int);
void Socketpair(int, int, int, int *);
void Writen(int, void *, size_t);
int Epoll_create(int size);
void Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int Epoll_wait(int epfd, struct epoll_event *events, int maxEvents, int timeout);
void epoll_add_event(int epfd, int fd, int flag);
void epoll_modify_event(int epfd, int fd, int flag);
void epoll_delete_event(int epfd, int fd);

// stdio
void Fclose(FILE *);
FILE *Fdopen(int, const char *);
char *Fgets(char *, int, FILE *);
FILE *Fopen(const char *, const char *);
void Fputs(const char *, FILE *);

// unix
void *Calloc(size_t, size_t);
void Close(int);
pid_t Fork(void);
void *Malloc(size_t);
void *Mmap(void *, size_t, int, int, int, off_t);
int Open(const char *, int, mode_t);
void Pipe(int *fds);
void Dup2(int fd1, int fd2);
ssize_t Read(int, void *, size_t);
void Sigaddset(sigset_t *, int);
void Sigdelset(sigset_t *, int);
void Sigemptyset(sigset_t *);
void Sigfillset(sigset_t *);
int Sigismember(const sigset_t *, int);
void Sigpending(sigset_t *);
void Sigprocmask(int, const sigset_t *, sigset_t *);
pid_t Wait(int *);
pid_t Waitpid(pid_t, int *, int);
void Write(int, void *, size_t);
Sigfunc *Signal(int, Sigfunc *);
int Fcntl(int fd, int cmd, int flag);
int Ioctl(int fd, int request, void *arg);
char *gf_time();

// Pthread
pthread_t Pthread_create(pthread_t *tid, pthread_attr_t *arrt, Threadfunc *func, void *arg);
void Pthread_join(pthread_t tid, void **retval);
pthread_t Pthread_self();
void Pthread_detach(pthread_t tid);
void Pthread_exit(void *retval);

// Pthread mutex
void Pthread_mutex_init(pthread_mutex_t *mptr, pthread_mutexattr_t *attr);
void Pthread_mutex_destory(pthread_mutex_t *mutex);
void Pthread_mutex_lock(pthread_mutex_t *mutex);
int Pthread_mutex_trylock(pthread_mutex_t *mutex);
void Pthread_mutex_unlock(pthread_mutex_t *mutex);

// Pthread cond
void Pthread_condattr_init(pthread_condattr_t *attr);
void Pthread_condattr_destroy(pthread_condattr_t *attr);
void Pthread_cond_signal(pthread_cond_t *cptr);
void Pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr);
void Pthread_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr, const struct timespec *tsptr);

// AIO
void Aio_read(struct aiocb *aiocbp);
void Aio_write(struct aiocb *aiocbp);
ssize_t Aio_return(struct aiocb *aiocbp);
int Aio_error(struct aiocb *aiocbp);
int Aio_suspend(const struct aiocb *const cblist[], int size, const struct timespec *timeout);
int Aio_cancel(int fd, struct aiocb *aiocbp);
int AioIsCancel(struct aiocb *aiocbp);
void Lio_listio(int mode, struct aiocb *list[], int nent, struct sigevent *sig);

// lib
void Inet_pton(int, const char *, void *);
const char *Inet_ntop(int, const void *, char *, size_t);
struct addrinfo *host_serv(const char *host, const char *serv, int family, int socktype);
struct addrinfo *Host_serv(const char *host, const char *serv, int family, int socktype);

// 错误处理
void err_sys(const char *, ...);  //系统调用出错
void err_quit(const char *, ...); //出错退出

// 封装库函数
int TcpConnect(char *ip, short port);
int TcpListen(short port);
int UdpConnect(char *ip, short port);
int UdpBind(short port);
void Usage(char *path);

// IPC
// POSXI semaphore
sem_t *Sem_open(const char *ipcName, int oflag, ...);
void Sem_close(sem_t *sem);
void Sem_unlink(const char *ipcName);
void Sem_wait(sem_t *sem);
int Sem_trywait(sem_t *sem);
void Sem_post(sem_t *sem);
int Sem_getvalue(sem_t *sem);
void Sem_init(sem_t *sem, int shared, unsigned int value);
void Sem_destory(sem_t *sem);

// mode
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/* default permissions for new files */

#endif
