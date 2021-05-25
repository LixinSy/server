#ifndef NET_REACTOR_H
#define NET_REACTOR_H

#include <string>
#include "net_event_module.h"
#include "net_sock_addr.h"
#include "net_tcp_server.h"
#include "net_tcp_connector.h"

class SockChannel;
class ParserCreator;

class Reactor
{
public:
    Reactor(EventModule *m);
    ~Reactor();
    void loop();
    bool register_acceptor(std::string host, uint16 port, uint16 backlog, ParserCreator *creator);
    TcpConnector* async_connect(const SockAddr& addr, ParserCreator *creator, IConnectorPtr callback);
    SockChannel* sync_connect(const SockAddr& addr, ParserCreator *creator, int timeout_ms);
private:
    EventModule *event_module_;
};

#endif // NET_REACTOR_H
