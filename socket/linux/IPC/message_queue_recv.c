#include "comm.h"

#define MQ_NAME "/messagequeue"
#define MQ_BUF_SIZE 16384 // bufsize 必须大于mq的msgsize

void ShowMq(mqd_t mqd)
{
    struct mq_attr attr;
    Mq_getattr(mqd, &attr);
    printf("flag:%ld maxmsg:%ld msgsize:%ld curmsg:%ld, maxpiro:%ld\n", attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs, sysconf(_SC_MQ_PRIO_MAX));
}

int main()
{
    mqd_t mqd = Mq_open(MQ_NAME, O_RDONLY);
    ShowMq(mqd);

    //发送方使用非阻塞IO
    struct mq_attr attr = {0};
    attr.mq_flags = O_NONBLOCK;
    Mq_setattr(mqd, &attr, NULL);

    ShowMq(mqd);

    char buf[MQ_BUF_SIZE];
    unsigned int prio;
    size_t n = Mq_receive(mqd, buf, MQ_BUF_SIZE, &prio); //默认阻塞

    printf("recv msg, prio:%u, len:%ld, content:%s\n", prio, n, buf);

    Mq_close(mqd);
}