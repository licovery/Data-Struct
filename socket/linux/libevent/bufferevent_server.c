#include "comm.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>

void ReadCallback(struct bufferevent *bev, void *ctx)
{
    struct evbuffer *input = bufferevent_get_input(bev);
    struct evbuffer *ouput = bufferevent_get_output(bev);
    evbuffer_add_buffer(ouput, input); // 把输入缓冲区的数据完全复制到输出缓冲区
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
    else
    {
        printf("unknow error\n");
    }
    bufferevent_free(bev);
}

void ListenerCallback(struct evconnlistener *evlistener, int connFd, struct sockaddr *addr, int socklen, void *arg)
{
    // evconnlistener_new_bind得到的connFd默认是nonblock的
    struct event_base *base = evconnlistener_get_base(evlistener);

    struct bufferevent *bufev = bufferevent_socket_new(base, connFd, BEV_OPT_CLOSE_ON_FREE); //bufev free的时候自动close fd
    bufferevent_setwatermark(bufev, EV_READ, 0, MAX_BUF_SIZE);                               //一次读取最大不超过MAX_BUF_SIZE
    bufferevent_enable(bufev, EV_READ | EV_WRITE);
    bufferevent_setcb(bufev, ReadCallback, NULL, ErrorCallback, NULL);
}

void AcceptErrorCallback(struct evconnlistener *evlistener, void *arg)
{
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr,
            "Got an error %d (%s) on the listener\n",
            err,
            evutil_socket_error_to_string(err));

    // accpet出错，退出主循环
    event_base_loopexit(evconnlistener_get_base(evlistener), NULL);
}

int main()
{
    struct addrinfo *addr = Host_serv(NULL, "30001", AF_INET, SOCK_STREAM);

    struct event_base *base = event_base_new();

    struct evconnlistener *evlistener = evconnlistener_new_bind(base,
                                                                ListenerCallback,
                                                                base,
                                                                LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                                                MAX_LISTEN_CON,
                                                                addr->ai_addr,
                                                                addr->ai_addrlen);
    evconnlistener_set_error_cb(evlistener, AcceptErrorCallback);

    event_base_dispatch(base);

    evconnlistener_free(evlistener);
    event_base_free(base);
    return 0;
}