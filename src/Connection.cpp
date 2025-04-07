#include "Connection.h"
#include "Server.h"

#include <spdlog/spdlog.h>
#include <cerrno>
#include <cstring>

Connection::Connection(Server *server, event_base *base, evutil_socket_t fd)
    : server_(server), fd_(fd)
{
    // 初始化bufferevent结构体
    bev_ = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (bev_ == nullptr)
    {
        spdlog::error("Failed to create bufferevent, fd:{}\treason:{}", fd, strerror(errno));
        return;
    }
    // 设置回调
    bufferevent_setcb(bev_, readCallback, nullptr, eventCallback, this);
    bufferevent_enable(bev_, EV_READ);
    spdlog::info("Connection created, fd:{}", fd);
}

Connection::~Connection()
{
    if (bev_)
    {
        bufferevent_free(bev_);
    }
    spdlog::info("Connection destroyed, fd:{}", fd_);
}

void Connection::readCallback(bufferevent *bev, void *ctx)
{
    // 指针转换
    auto self = static_cast<Connection *>(ctx);
    char buffer[1024] = {0};
    // Echo
    while (bufferevent_read(bev, buffer, sizeof(buffer)))
    {
        bufferevent_write(bev, buffer, strlen(buffer));
    }
}

void Connection::eventCallback(bufferevent *bev, short events, void *ctx)
{
    // 指针转换
    auto self = static_cast<Connection *>(ctx);
    // 如果客户端关闭或者出现错误
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR))
    {
        // 通知服务器
        self->server_->handleConnectionClosed(self->fd_);
    }
}
