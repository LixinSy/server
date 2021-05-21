#ifndef NET_TCP_SERVER_H
#define NET_TCP_SERVER_H

#include "net_socket.h"
#include "net_sock_addr.h"
#include "net_event_module.h"
#include "net_connection.h"

class TcpServer: NetEvent
{
public:
    TcpServer(const std::string &host, uint16 port, ParserCreator *creator, EventModule *module);
    virtual ~TcpServer();
    virtual int handle_input() override;
    bool start();
private:
    std::string host_;
    uint16 port_;
    Socket *sock_;
    ParserCreator *parser_creator_;
};

#endif // NET_TCP_SERVER_H
