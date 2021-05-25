#ifndef NET_EPOLL_H
#define NET_EPOLL_H

#include "comm_inc.h"
#include "net_event.h"
#include "net_event_module.h"
#include "net_tcp_server.h"

class Epoll: public EventModule
{
public:
    Epoll(uint32 max_event_size);
    virtual ~Epoll();
    virtual void wait_event(NetEventList *event_list) override;
    virtual void update_event(NetEvent *net_event) override;
    virtual void process_events() override;
private:
    void _operate(int opt, NetEvent *net_event);

    int epfd_;
    struct epoll_event *events_;
};

#endif // NET_EPOLL_H
