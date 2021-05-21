#ifndef NET_EVENT_H
#define NET_EVENT_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <atomic>
#include "comm_inc.h"
#include "def.h"


class EventSlot;
class EventModule;
class Epoll;

class NetEvent
{
    friend EventModule;
    friend Epoll;
    enum State: int { NEW = 0, ADDED, DELETEED, };
public:
    NetEvent(EventModule *m);
    virtual ~NetEvent() {}
    virtual int handle_input()  { return LX_OK; }
    virtual int handle_output() { return LX_OK; }
    virtual int handle_close()  { return LX_OK; }
    void enable_input()   { regist_events_ |= EPOLLIN; }
    void disable_input()  { regist_events_ &= ~EPOLLIN; }
    void enable_output()  { regist_events_ |= EPOLLOUT; }
    void disable_output() { regist_events_ &= ~EPOLLOUT; }
    void disable_all()    { regist_events_ = 0; }
    void attach_slot(EventSlot *s);
    void detach_slot(EventSlot *s);
    void add_ref() { ++ref_; }
    void release();
protected:
    int fd_;
    std::atomic_int ref_;
    int state_;
    int regist_events_;
    int active_events_;
    EventSlot *slot_; //self pos
    EventModule *event_module_;
};


#endif // NET_EVENT_H
