#ifndef COMM_H
#define COMM_H

// linux(unix)内核
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

// standard C
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

// 限制值
#define MAX_LISTEN_CON 16
#define MAX_BUF_SIZE 4096
#define MAXLINE 1024
#define MAX_CONNECTION 1024

// 端口
#define DAYTIME_PORT 30000
#define ECHO_PORT 30001
#define SERVER_PORT 9877

void CheckStatus(int status);
void CheckPointer(void *ptr);
void ShowSockaddr(const struct sockaddr_in *addr);

#define	SA	struct sockaddr
typedef	void	Sigfunc(int);	/* for signal handlers */
#define	bzero(ptr,n)		memset(ptr, 0, n)

// wapper函数

// socket
int		 Accept(int, SA *, socklen_t *);
void	 Bind(int, const SA *, socklen_t);
void	 Connect(int, const SA *, socklen_t);
void	 Getpeername(int, SA *, socklen_t *);
void	 Getsockname(int, SA *, socklen_t *);
void	 Getsockopt(int, int, int, void *, socklen_t *);
void	 Listen(int, int);
int      Poll(struct pollfd *, unsigned long, int);
ssize_t	 Readline(int, void *, size_t);
ssize_t	 Readn(int, void *, size_t);
ssize_t	 Recv(int, void *, size_t, int);
ssize_t	 Recvfrom(int, void *, size_t, int, SA *, socklen_t *);
ssize_t	 Recvmsg(int, struct msghdr *, int);
int		 Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
void	 Send(int, const void *, size_t, int);
void	 Sendto(int, const void *, size_t, int, const SA *, socklen_t);
void	 Sendmsg(int, const struct msghdr *, int);
void	 Setsockopt(int, int, int, const void *, socklen_t);
void	 Shutdown(int, int);
int		 Sockatmark(int);
int		 Socket(int, int, int);
void	 Socketpair(int, int, int, int *);
void	 Writen(int, void *, size_t);

// stdio
void	 Fclose(FILE *);
FILE	*Fdopen(int, const char *);
char	*Fgets(char *, int, FILE *);
FILE	*Fopen(const char *, const char *);
void	 Fputs(const char *, FILE *);

// unix
void	*Calloc(size_t, size_t);
void	 Close(int);
pid_t	 Fork(void);
void	*Malloc(size_t);
void	*Mmap(void *, size_t, int, int, int, off_t);
int		 Open(const char *, int, mode_t);
void	 Pipe(int *fds);
ssize_t	 Read(int, void *, size_t);
void	 Sigaddset(sigset_t *, int);
void	 Sigdelset(sigset_t *, int);
void	 Sigemptyset(sigset_t *);
void	 Sigfillset(sigset_t *);
int		 Sigismember(const sigset_t *, int);
void	 Sigpending(sigset_t *);
void	 Sigprocmask(int, const sigset_t *, sigset_t *);
pid_t	 Wait(int *);
pid_t	 Waitpid(pid_t, int *, int);
void	 Write(int, void *, size_t);
Sigfunc *Signal(int, Sigfunc *);

// lib
void			 Inet_pton(int, const char *, void *);
const char	*Inet_ntop(int, const void *, char *, size_t);

// 错误处理
void	 err_sys(const char *, ...);   //系统调用出错
void	 err_quit(const char *, ...);  //出错退出

#endif