#include "comm.h"

#define MQ_NAME "/messagequeue"
#define MQ_BUF_SIZE 16384
#define DEFALUT_PRIO 16

void ShowMq(mqd_t mqd)
{
    struct mq_attr attr;
    Mq_getattr(mqd, &attr);
    printf("flag:%ld maxmsg:%ld msgsize:%ld curmsg:%ld, maxpiro:%ld\n", attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs, sysconf(_SC_MQ_PRIO_MAX));
}

int main()
{
    // mqd_t mqd = Mq_open(MQ_NAME, O_RDWR | O_CREAT | O_EXCL, FILE_MODE, NULL);
    mqd_t mqd = Mq_open(MQ_NAME, O_RDWR | O_CREAT, FILE_MODE, NULL);
    ShowMq(mqd);

    //接收方使用阻塞IO
    struct mq_attr attr = {0};
    attr.mq_flags = O_NONBLOCK;
    Mq_setattr(mqd, &attr, NULL);
    ShowMq(mqd);

    char buf[MQ_BUF_SIZE];
    snprintf(buf, MQ_BUF_SIZE, "no1 hello");
    Mq_send(mqd, buf, strlen(buf) + 1, DEFALUT_PRIO);

    snprintf(buf, MQ_BUF_SIZE, "no2 world");
    Mq_send(mqd, buf, strlen(buf) + 1, DEFALUT_PRIO);

    ShowMq(mqd);

    Mq_close(mqd);
    // Mq_unlink(MQ_NAME);
}