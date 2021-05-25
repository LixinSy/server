#include "net_tcp_server.h"
#include "net_sock_channel.h"
#include "net_parser.h"
#include "net_socket.h"


TcpServer::TcpServer(const std::string &host, uint16 port, uint16 backlog, ParserCreator *creator, EventModule *module)
    : NetEvent(module),
      host_(host),
      port_(port),
      backlog_(backlog),
      parser_creator_(creator)
{
}

TcpServer::~TcpServer() {
    if (sock_) delete sock_;
    if (parser_creator_) delete parser_creator_;
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
    if (::bind(sock_->fd(), (struct sockaddr*)&addrin, sizeof (struct sockaddr)) == -1) {
        fprintf(stderr, "bind err\n");
        return false;
    }
    if (::listen(sock_->fd(), backlog_) == -1) {
        fprintf(stderr, "listen err\n");
        return false;
    }
    fd_ = sock_->fd();
    enable_input();
    event_module_->update_event(this);
    return true;
}

int TcpServer::handle_input() {
    SockAddr addr(AF_INET);
    Socket *sock = Socket::accept(sock_->fd(), addr);
    if (!sock || !sock->set_nonblock()) {
        return LX_ERROR;
    }
    SockChannel *chan = new SockChannel(event_module_, sock, parser_creator_->create());
    chan->enable_input();
    event_module_->update_event(chan);
    return LX_OK;
}

int TcpServer::handle_output() {
    return LX_OK;
}
int TcpServer::handle_close() {
    return LX_OK;
}

