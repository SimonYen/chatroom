#include "Server.h"
#include "Connection.h"
#include "Listener.h"

#include <spdlog/spdlog.h>
#include <cstdlib>

Server::Server(int port)
    : port_(port), base_(event_base_new()), listener_(nullptr)
{
    if (!base_)
    {
        spdlog::error("Failed to create event_base!");
        exit(EXIT_FAILURE);
    }
    // 创建监听器
    listener_ = new Listener(this, base_, port_);
}

Server::~Server()
{
    // 清理所有连接
    for (auto &fd2conn : connections_)
    {
        delete fd2conn.second;
    }
    // 删除监听器
    delete listener_;
    // 释放event_base
    event_base_free(base_);
    spdlog::info("Server deconstructed.");
}

void Server::run()
{
    spdlog::info("Server is running.");
    // 交给libevent分发事件处理
    event_base_dispatch(base_);
}

void Server::handleNewConnection(evutil_socket_t fd)
{
    auto conn = new Connection(this, base_, fd);
    // 存到哈希中
    connections_[fd] = conn;
}

void Server::handleConnectionClosed(evutil_socket_t fd)
{
    // 在哈希中查找
    auto it = connections_.find(fd);
    // 如果存在
    if (it != connections_.end())
    {
        delete it->second;
        connections_.erase(it);
    }
}
