#include "net_tcp_server.h"
#include <functional>
#include "net_sock_channel.h"


TcpServer::TcpServer(const std::string &host, uint16 port, ParserCreator *creator, EventModule *module)
    : NetEvent(module),
      host_(host),
      port_(port),
      parser_creator_(creator)
{
}

TcpServer::~TcpServer() {
    delete sock_;
    delete parser_creator_;
}

bool TcpServer::start() {
    if (!parser_creator_) {
        return false;
    }
    if (!event_module_) {
        return false;
    }
    sock_ = Socket::open(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sock_) {
        return false;
    }
    sock_->set_nonblock();
    struct sockaddr_in addrin;
    addrin.sin_addr.s_addr = inet_addr(host_.c_str());
    addrin.sin_port = port_;
    if (::bind(sock_->fd(), (struct sockaddr*)&addrin, sizeof (struct sockaddr))) {
        fprintf(stderr, "bind err\n");
        return false;
    }
    if (::listen(sock_->fd(), 1024)) {
        fprintf(stderr, "listen err\n");
        return false;
    }
    fd_ = sock_->fd();
    enable_input();
    event_module_->update_event(this);
    return true;
}

int TcpServer::handle_input() {

    SockAddr *addr = new SockAddr(AF_INET);
    Socket *sock = Socket::accept(sock_->fd(), *addr);
    sock->set_nonblock();
    SockChannel *chan = new SockChannel(event_module_, sock, parser_creator_->create());
    chan->enable_input();
    event_module_->update_event(chan);
    return 0;
}

