#ifndef NET_SOCK_CHANNEL_H
#define NET_SOCK_CHANNEL_H

#include <memory>
#include "core_mutex.h"
#include "net_event.h"


class Socket;
class Parser;
class BaseMessage;
class Session;

using SessionPtr = std::shared_ptr<Session>;
using SessionWPtr = std::weak_ptr<Session>;

class SockChannel: public NetEvent
{
public:
    SockChannel(EventModule *m, Socket *sock, Parser *parser);
    virtual ~SockChannel();
    virtual int handle_input() override;
    virtual int handle_output() override;
    virtual int handle_close() override;
    void attach_session(SessionPtr session_ptr);
    void read();
    void write(BaseMessage* msg);
private:
    Socket *sock_;
    Parser *parser_;
    Mutex recving_mutex_;
    Mutex sending_mutex_;
    SessionWPtr tied_session_;
};

#endif // NET_SOCK_CHANNEL_H
