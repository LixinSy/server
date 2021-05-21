#ifndef NET_REACTOR_H
#define NET_REACTOR_H

#include <string>
#include <vector>
#include "net_event_module.h"
#include "net_tcp_server.h"
#include "net_parser.h"
#include "core_thread_pool.h"

class Reactor
{
public:
    Reactor(EventModule *m);
    ~Reactor();
    void loop() {
        event_module_->process_event();

    }
    bool register_acceptor(std::string host, uint16 port, ParserCreator *creator) {
        TcpServer *svr = new TcpServer(host, port, creator, event_module_);
        if (!svr->start()) {
            delete svr;
            return false;
        }
        return true;
    }
private:
    EventModule *event_module_;
};

#endif // NET_REACTOR_H
