#include "comm.h"
#include <event2/event.h>
#include <event2/bufferevent.h>


void ReadCallback(struct bufferevent *bev, void *ctx)
{
    char buf[MAX_BUF_SIZE];
    int n = bufferevent_read(bev, buf, MAX_BUF_SIZE);
    if (n > MAX_BUF_SIZE)
    {
        printf("read too much data\n");
        n = MAX_BUF_SIZE;
    }
    bufferevent_write(bev, buf, n);
}

void ErrorCallback(struct bufferevent *bev, short event, void *ctx)
{
    if (event & BEV_EVENT_EOF) //对端关闭
    {
        printf("clinet close\n");
    }
    else if (event & BEV_EVENT_ERROR)
    {
        printf("network error\n");
    }
    bufferevent_free(bev);
}

void AcceptCallback(int fd, short event, void *arg)
{
    struct event_base *base = (struct event_base *)arg;

    int connFd = Accept(fd, NULL, NULL);

    struct bufferevent *bufev = bufferevent_socket_new(base, connFd, BEV_OPT_CLOSE_ON_FREE); //bufev free的时候自动close fd
    bufferevent_setwatermark(bufev, EV_READ, 0, MAX_BUF_SIZE); //一次读取最大不超过MAX_BUF_SIZE
    bufferevent_enable(bufev, EV_READ|EV_WRITE);
    bufferevent_setcb(bufev, ReadCallback, NULL, ErrorCallback, NULL);
}


int main()
{
    int listenFd = TcpListen(ECHO_PORT);

    struct event_base *base = event_base_new();

    struct event *listenEvent = event_new(base, listenFd, EV_READ|EV_PERSIST, AcceptCallback, base);
    event_add(listenEvent, NULL);

    event_base_dispatch(base);

    event_free(listenEvent);
    event_base_free(base);
}