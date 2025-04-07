#include "Listener.h"
#include "Server.h"
#include <cerrno>
#include <cstring>

#include <spdlog/spdlog.h>

Listener::Listener(Server *server, event_base *base, int port)
{
    // 初始化地址
    sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // 绑定本地地址
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    lev_ = evconnlistener_new_bind(
        base,
        acceptCallback,
        server,
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
        -1,
        (sockaddr *)(&addr),
        sizeof(addr));
    if (lev_)
    {
        spdlog::info("Listener created successful, listening on port:{}", port);
    }
    else
    {
        spdlog::error("Failed to create listener:{}", strerror(errno));
    }
}

Listener::~Listener()
{
    if (lev_)
    {
        evconnlistener_free(lev_);
    }
    spdlog::info("Listener destroyed.");
}

void Listener::acceptCallback(evconnlistener *lev, evutil_socket_t fd, sockaddr *addr, int socklen, void *ctx)
{
    // 指针转换
    auto server = static_cast<Server *>(ctx);
    // 调用回调函数
    server->handleNewConnection(fd);
}
