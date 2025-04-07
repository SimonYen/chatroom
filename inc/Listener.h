#pragma once

#include <event2/event.h>
#include <event2/listener.h>

class Server; // 前置声明

class Listener
{
public:
    Listener(Server *server, event_base *base, int port);
    ~Listener();

private:
    // 有客户端到来时的回调函数
    static void acceptCallback(evconnlistener *lev, evutil_socket_t fd, sockaddr *addr, int socklen, void *ctx);
    evconnlistener *lev_;
};