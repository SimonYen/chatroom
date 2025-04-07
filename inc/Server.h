#pragma once

#include <unordered_map>
#include <event2/event.h>
#include <event2/listener.h>

// 前置声明
class Listener;
class Connection;

class Server
{
public:
    Server(int port);
    ~Server();
    void run();

    // 处理新连接(供Listener回调)
    void handleNewConnection(evutil_socket_t fd);
    // 处理连接关闭(供Connection回调)
    void handleConnectionClosed(evutil_socket_t fd);

private:
    event_base *base_;
    int port_;
    Listener *listener_;
    std::unordered_map<evutil_socket_t, Connection *> connections_; // 管理所有连接
};