#ifndef NET_SESSION_H
#define NET_SESSION_H

#include <memory>


class SockChannel;

class Session: public std::enable_shared_from_this<Session>
{
public:
    Session();
    virtual ~Session();
    bool open_session(SockChannel* chan);
    bool close_session();
    bool is_alive();
protected:
    virtual void on_opened_session() = 0;
    virtual void on_closed_session() = 0;
private:
    SockChannel *channel_;
};

#endif // NET_SESSION_H
