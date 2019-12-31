#include "comm.h"

static int sockfd;
static int inputEOF = 0;

void ReadInput()
{
    char buf[MAX_BUF_SIZE];
    int n;

    while ((n = Read(STDIN_FILENO, buf, MAX_BUF_SIZE)) > 0)
    {
        Writen(sockfd, buf, n);
    }

    Shutdown(sockfd, SHUT_WR);
    inputEOF = 1;
}


void* ClientProc(void *arg)
{
    char buf[MAX_BUF_SIZE];
    int n;

    while (1)
    {
        if ((n = Read(sockfd, buf, MAX_BUF_SIZE)) == 0)
        {
            if (!inputEOF)
            {
                fprintf(stderr, "server close\n");
                exit(-1); // 让主线程也结束
            }
            break;
        }
        Writen(STDOUT_FILENO, buf, n);
    }

    return NULL;
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

    sockfd = TcpConnect(ip, port);

    pthread_t tid;
    Pthread_create(&tid, NULL, ClientProc, NULL);

    ReadInput();

    Pthread_join(tid, NULL);
    return 0;
}