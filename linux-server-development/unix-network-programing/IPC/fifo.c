#include "comm.h"

//fifo是命名管道，可以用于无亲缘关系的进程

//name的规则是pathname
#define FIFO1 "/tmp/fifo1" //父进程写，子进程读
#define FIFO2 "/tmp/fifo2" //父进程读，子进程写

void Client(int readFd, int writeFd)
{
    char buf[MAX_BUF_SIZE];
    Fgets(buf, MAX_BUF_SIZE, stdin); //读入了换行符
    int len = strlen(buf);
    buf[len - 1] = '\0';
    Write(writeFd, buf, len);
    printf("client send req: %s\n", buf);
    Read(readFd, buf, len);
    printf("client recv rsp: %s\n", buf);
}

void Server(int readFd, int writeFd)
{
    char buf[MAX_BUF_SIZE];
    int n = Read(readFd, buf, MAX_BUF_SIZE);
    printf("server recv req: %s\n", buf);
    Write(writeFd, buf, n);
    printf("server send rsp: %s\n", buf);
}

int main()
{
    int readFd, writeFd;
    pid_t childpid;

    if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST))
    {
        err_sys("mkfifo error");
    }

    if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST))
    {
        err_sys("mkfifo error");
    }

    if ((childpid = Fork()) == 0) //子进程
    {
        readFd = Open(FIFO1, O_RDONLY, 0);
        writeFd = Open(FIFO2, O_WRONLY, 0);
        Server(readFd, writeFd);

        // Close(readFd);
        // Close(writeFd);
        // unlink(FIFO1);
        // unlink(FIFO2);

        exit(0); // exit会自动关闭打开的文件描述符和fifo
    }

    // 父进程
    // open默认是阻塞的，详见unpv22e p45
    // 以只读方式打开会阻塞直到有进程打开来写，同理以只写方式打开会阻塞直到有进程打开来读

    // 如果是阻塞方式，不可能在一个进程分别打开只读和只写，因为会相互阻塞

    // 互换两个FIFO的打开顺序会导致死锁
    writeFd = Open(FIFO1, O_WRONLY, 0);
    readFd = Open(FIFO2, O_RDONLY, 0);
    Client(readFd, writeFd);

    Waitpid(childpid, NULL, 0);

    Close(readFd);
    Close(writeFd);
    unlink(FIFO1);
    unlink(FIFO2);

    return 0;
}