#ifndef NET_EVENT_MODULE_H
#define NET_EVENT_MODULE_H

#include <vector>
#include "def.h"
#include "net_event.h"
#include "core_thread_pool.h"


using NetEventList = std::vector<NetEvent*>;

struct EventSlot
{
    NetEvent    *event;
    EventSlot   *free_list;
};

class EventModule
{
public:
    EventModule(uint32 max_event_size);
    virtual ~EventModule();
    virtual void wait_event(NetEventList *) = 0;
    virtual void update_event(NetEvent *) = 0;
    virtual void process_events() = 0;
    bool init(uint32 thread_num);
    EventSlot *alloc_slot();
    void free_slot(EventSlot *s);
    bool add_event(NetEvent *e);
    bool del_event(NetEvent *e);
    bool push_task(ThreadPool::TaskFunc func, void *arg) {
        if (thread_pool_ && thread_pool_->add_task(func, arg) != LX_OK) {
            return true;
        }
        return false;
    }
protected:
    uint32 max_event_size_;
    EventSlot *event_list_;
    EventSlot *free_list_;
    ThreadPool *thread_pool_;
};

#endif // NET_EVENT_MODULE_H
