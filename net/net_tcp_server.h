#ifndef NET_TCP_SERVER_H
#define NET_TCP_SERVER_H

#include "net_event_module.h"

class Socket;
class ParserCreator;

class TcpServer: NetEvent
{
public:
    TcpServer(const std::string &host, uint16 port, uint16 backlog, ParserCreator *creator, EventModule *module);
    virtual ~TcpServer();
    virtual int handle_input() override;
    virtual int handle_output() override;
    virtual int handle_close() override;
    bool start();
private:
    std::string host_;
    uint16 port_;
    uint16 backlog_;
    Socket *sock_;
    ParserCreator *parser_creator_;
};

#endif // NET_TCP_SERVER_H
