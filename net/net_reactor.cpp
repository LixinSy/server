#include "net_reactor.h"
#include "net_socket.h"
#include "net_parser.h"
#include "net_sock_channel.h"


Reactor::Reactor(EventModule *m)
    : event_module_(m)
{
}

Reactor::~Reactor() {
    if (event_module_)  delete event_module_;
}

void Reactor::loop() {
    event_module_->process_events();
}

bool Reactor::register_acceptor(std::string host, uint16 port, uint16 backlog, ParserCreator *creator) {
    TcpServer *svr = new TcpServer(host, port, backlog, creator, event_module_);
    if (!svr->start()) {
        delete svr;
        return false;
    }
    return true;
}

TcpConnector* Reactor::async_connect(const SockAddr& addr, ParserCreator *creator, IConnectorPtr callback) {
    Socket *sock = Socket::open(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sock) {
        delete creator;
        return nullptr;
    }
    if (!sock->set_nonblock()) {
        delete sock;
        delete creator;
        return nullptr;
    }
    TcpConnector *connector = new TcpConnector(event_module_, sock, creator);
    connector->regist_callback(callback);
    int ret = ::connect(sock->fd(), (struct sockaddr*)(addr.get_addr()), sizeof (struct sockaddr));
    if (ret == 0) {
        connector->set_state(TcpConnector::State::CONNECTED);
        return connector;
    } else {
        int err = errno;
        if (err == EINPROGRESS) {
            connector->set_state(TcpConnector::State::CONNECTING);
            connector->enable_output();
            event_module_->update_event(connector);
            return connector;
        } else {
            delete sock;
            delete connector;
            return nullptr;
        }
    }
    return nullptr;
}

SockChannel* Reactor::sync_connect(const SockAddr& addr, ParserCreator *creator, int timeout_ms) {
    Socket *sock = Socket::open(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sock) {
        delete creator;
        return nullptr;
    }
    if (!sock->set_nonblock()) {
        delete sock;
        delete creator;
        return nullptr;
    }
    int ret = ::connect(sock->fd(), (struct sockaddr*)(addr.get_addr()), sizeof (struct sockaddr));
    if (ret == 0) {
        SockChannel *chan = new SockChannel(event_module_, sock, creator->create());
        chan->enable_input();
        event_module_->update_event(chan);
        delete creator;
        return chan;
    } else {
        int err = errno;
        if (err == EINPROGRESS) {
            int epoll_size = 3;
            int epfd = ::epoll_create(epoll_size);
            epoll_event ev;
            ev.events = EPOLLOUT;
            ev.data.fd = sock->fd();
            if (::epoll_ctl(epfd, EPOLL_CTL_MOD, sock->fd(), &ev) < 0) {
                delete sock;
                delete creator;
                ::close(epfd);
                return nullptr;
            }
            epoll_event events[epoll_size];
            int n = ::epoll_wait(epfd, events, epoll_size, timeout_ms);
            if (n > 0) {
                for (int i = 0; i < n; ++i) {
                    if (events[i].events & EPOLLOUT) {
                        if (events[i].data.fd == sock->fd()) {
                            SockChannel *chan = new SockChannel(event_module_, sock, creator->create());
                            chan->enable_input();
                            event_module_->update_event(chan);
                            ::close(epfd);
                            delete creator;
                            return chan;
                        }
                    }
                }
            } else {
                delete sock;
                delete creator;
                close(epfd);
                return nullptr;
            }
        } else {
            delete sock;
            delete creator;
            return nullptr;
        }
    }
    return nullptr;
}
