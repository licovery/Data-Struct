#include "comm.h"

#define LOOP 5

int main()
{

    int fd[2]; //fd[0]父进程  fd[1]子进程
    Socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
    pid_t pid;

    if ((pid = Fork()) == 0) //子进程
    {
        Close(fd[0]);

        // 子进程收到父进程的消息，+1后返回
        for (int i = 0; i < LOOP; i++)
        {
            int msg;
            Readn(fd[1], &msg, sizeof(int));
            sleep(1);
            msg++;
            Writen(fd[1], &msg, sizeof(int));
        }
        exit(0);
    }

    // 父进程
    Close(fd[1]);
    int msg = 0;

    for (int i = 0; i < LOOP; i++, msg++)
    {
        fprintf(stderr, "send msg:(%d) to child\n", msg);
        Writen(fd[0], &msg, sizeof(int));
        Readn(fd[0], &msg, sizeof(int));
        fprintf(stderr, "recv msg:(%d) from child\n", msg);
    }

    return 0;
}