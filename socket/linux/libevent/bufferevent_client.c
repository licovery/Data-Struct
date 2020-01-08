#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/event.h"
#include "comm.h"

int stdinEof = 0;

void StdinCallback(int fd, short event, void *arg)
{
    struct bufferevent *bev = (struct bufferevent *)arg;
    if (event & EV_CLOSED)
    {
        int connFd = bufferevent_getfd(bev);
        Shutdown(connFd, SHUT_WR);
        stdinEof = 1;
    }
    else if (event & EV_READ)
    {
        char buf[MAX_BUF_SIZE];
        int n = Read(fd, buf, MAX_BUF_SIZE);
        bufferevent_write(bev, buf, n);
    }
    else
    {
        printf("unknow event\n");
    }
}

void ReadCallback(struct bufferevent *bev, void *arg)
{
    char buf[MAX_BUF_SIZE];
    int n = bufferevent_read(bev, buf, MAX_BUF_SIZE);
    Writen(STDOUT_FILENO, buf, n);
}

void EventCallback(struct bufferevent *bev, short event, void *arg)
{
    if (event & BEV_EVENT_CONNECTED)
    {
        printf("connect to server\n");
    }
    else if (event & BEV_EVENT_EOF && !stdinEof)
    {
        printf("server close\n");
        event_base_loopbreak(bufferevent_get_base(bev));
    }
    else if (event & BEV_EVENT_ERROR)
    {
        perror("connBufEv");
        event_base_loopbreak(bufferevent_get_base(bev));
    }
}

int main()
{
    struct addrinfo *addr = Host_serv("127.0.0.1", "30001", AF_INET, SOCK_STREAM);

    struct event_base *base = event_base_new();

    struct bufferevent *connBufEv = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_enable(connBufEv, EV_READ | EV_WRITE);
    bufferevent_setcb(connBufEv, ReadCallback, NULL, EventCallback, NULL);
    bufferevent_socket_connect(connBufEv, addr->ai_addr, addr->ai_addrlen);

    struct event *stdinEvent = event_new(base,
                                         STDIN_FILENO,
                                         EV_READ | EV_CLOSED | EV_PERSIST,
                                         StdinCallback,
                                         connBufEv);
    event_add(stdinEvent, NULL);

    event_base_dispatch(base);

    bufferevent_free(connBufEv);
    event_free(stdinEvent);
    event_base_free(base);

    return 0;
}