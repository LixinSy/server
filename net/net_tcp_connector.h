#ifndef NET_TCP_CONNECTOR_H
#define NET_TCP_CONNECTOR_H

#include <memory>
#include "net_event.h"


class Socket;
class Session;
class ParserCreator;
class SockChannel;

using SessionPtr = std::shared_ptr<Session>;
using SessionWPtr = std::weak_ptr<Session>;

struct IConnector
{
    virtual void on_connected() = 0;
    virtual void on_disconnected() = 0;
    virtual void on_failed() = 0;
};
using IConnectorPtr = std::shared_ptr<IConnector>;
using IConnectorWPtr = std::weak_ptr<IConnector>;

class TcpConnector: public NetEvent
{
public:
    enum State { IDLE=0, CONNECTING, CONNECTED, DISCONNECT };
public:
    TcpConnector(EventModule *m, Socket *sock, ParserCreator *creator);
    virtual ~TcpConnector();
    virtual int handle_input() override;
    virtual int handle_output() override;
    virtual int handle_close() override;
    void set_state(State s) {state_ = s;}
    State get_state() {return state_;}
    void regist_callback(IConnectorPtr callback);
    SockChannel *get_channel();
private:
    State state_;
    Socket *sock_;
    ParserCreator *parser_creator_;
    IConnectorWPtr conn_call_back_;
};

#endif // NET_TCP_CONNECTOR_H
