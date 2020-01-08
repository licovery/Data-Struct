#include <event2/event.h>
#include <event2/listener.h>
#include "comm.h"

struct timeval tv = {5, 0};

void ReadCallBack(int fd, short event, void *arg)
{
    struct event *readEvent = (struct event *)arg;
    if (event & EV_TIMEOUT)
    {
        printf("fd:%d client unactive\n", fd);
        event_del(readEvent);
        event_free(readEvent);
        Close(fd);
    }
    else if (event & EV_CLOSED)
    {
        printf("fd:%d client close\n", fd);
        event_del(readEvent);
        event_free(readEvent);
        Close(fd);
    }
    else if (event & EV_READ)
    {
        printf("fd:%d read from client\n", fd);
        char buf[MAX_BUF_SIZE];
        int n = Read(fd, buf, MAX_BUF_SIZE);
        Writen(fd, buf, n);
    }
}

void AcceptCallBack(int fd, short event, void *arg)
{
    struct event_base *base = (struct event_base *)arg;

    int connFd = Accept(fd, NULL, NULL);

    struct event *readEvent = event_new(base, connFd, EV_READ | EV_CLOSED | EV_PERSIST, ReadCallBack, event_self_cbarg());
    struct timeval connActiveInternal = {10, 0}; // 10秒没有活跃则超时断开
    event_add(readEvent, &connActiveInternal);
}

void SigIntCallback(int signo, short event, void *arg)
{
    struct event_base *base = (struct event_base *)arg;
    printf("server exit because SIGINT\n");
    event_base_loopexit(base, NULL);
}

void TimeoutCallBack(int fd, short event, void *arg)
{
    struct event *timerEvent = (struct event *)arg;
    printf("%s\n", gf_time());
    event_add(timerEvent, &tv);
}

int main()
{
    int listenFd = TcpListen(ECHO_PORT);
    // 创建Reactor实例
    struct event_base *base = event_base_new();

    //创建并注册IO事件处理器
    struct event *listenEvent = event_new(base, listenFd, EV_READ | EV_PERSIST, AcceptCallBack, base);
    event_add(listenEvent, NULL);

    // 创建并注册信号事件处理器
    struct event *signalEvent = evsignal_new(base, SIGINT, SigIntCallback, base);
    event_add(signalEvent, NULL);

    // 创建并注册定时器事件处理器
    struct event *timerEvent = evtimer_new(base, TimeoutCallBack, event_self_cbarg());
    event_add(timerEvent, &tv);

    // 事件分发循环
    event_base_dispatch(base);

    // 释放
    event_free(listenEvent);
    event_free(signalEvent);
    event_free(timerEvent);
    event_base_free(base);
    return 0;
}