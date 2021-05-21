#ifndef NET_SESSION_H
#define NET_SESSION_H

#include "net_sock_channel.h"


class Session: public std::enable_shared_from_this<Session>
{
public:
    Session();
    virtual ~Session();
    bool open_session(SockChannel* chan) {
        channel_ = chan;
        channel_->attach_session(shared_from_this());
        on_opened_session();
        return true;
    }
    bool close_session() {
        channel_ = nullptr;
        on_closed_session();
        return true;
    }
    bool is_alive() {
        return channel_ != nullptr;
    }
protected:
    virtual void on_opened_session() = 0;
    virtual void on_closed_session() = 0;
private:
    SockChannel *channel_;
};

#endif // NET_SESSION_H
