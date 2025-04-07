#pragma once

#include <event2/event.h>
#include <event2/bufferevent.h>

class Server; // 前置申明

// 客户端的连接抽象成连接类
class Connection
{
public:
    Connection(Server *server, event_base *base, evutil_socket_t fd);
    ~Connection();

private:
    // 读回调函数
    static void readCallback(bufferevent *bev, void *ctx);
    // 事件回调函数
    static void eventCallback(bufferevent *bev, short events, void *ctx);

private:
    Server *server_; // 用于回调
    bufferevent *bev_;
    evutil_socket_t fd_;
};