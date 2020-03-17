#include "comm.h"



#define MAX_EVENT 2
#define MAX_LINE 10
char *p = "aaaa\nbbbb\n";

int main()
{
    int fd[2][2];
    pipe(fd[0]); // read:f[0][0]   write:fd[0][1]
    pipe(fd[1]); // read:f[1][0]   write:fd[1][1]

    // 子进程1写fd[0][1]  LT
    if (Fork() == 0)
    {
        int ltfd = fd[0][1];
        while (1)
        {
            Write(ltfd, p, MAX_LINE);
            sleep(5);
        }
        exit(0);
    }

    // 子进程2写fd[1][1]  ET
    if (Fork() == 0)
    {
        int etfd = fd[1][1];
        while (1)
        {
            Write(etfd, p, MAX_LINE);
            sleep(5);
        }
        exit(0);
    }

    // 父进程读数据

    char buf[MAX_BUF_SIZE];

    int epfd = Epoll_create(MAX_EVENT);
    epoll_add_event(epfd, fd[0][0], EPOLLIN);           // LT
    epoll_add_event(epfd, fd[1][0], EPOLLIN|EPOLLET);   // ET

    struct epoll_event events[MAX_EVENT];
    while (1)
    {
        int ready = Epoll_wait(epfd, events, MAX_EVENT, -1);
        for (int i = 0; i < ready; i++)
        {

            if (events[i].data.fd == fd[0][0])
            {
                printf("LT: %ld\n", time(NULL));
            }
            else if (events[i].data.fd == fd[1][0])
            {
                printf("ET: %ld\n", time(NULL));
            }
             
            int n = Read(events[i].data.fd, buf, MAX_LINE / 2);
            Writen(STDOUT_FILENO, buf, n);
        }
    }
}