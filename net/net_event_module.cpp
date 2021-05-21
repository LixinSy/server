#include "net_event_module.h"

EventModule::EventModule(uint32 max_event_size)
    : max_event_size_(max_event_size)
{

}

EventModule::~EventModule() {
    if (event_list_) {
        delete [] event_list_;
    }
    if (thread_pool_) {
        delete thread_pool_;
    }
}

bool EventModule::init(uint32 thread_num) {
    event_list_ = new EventSlot[max_event_size_];
    for (size_t i = 0; i < max_event_size_-1; ++i) {
        event_list_[i].event = nullptr;
        event_list_[i].free_list = event_list_ + i + 1;
    }
    event_list_[max_event_size_-1].free_list = nullptr;
    free_list_ = event_list_;
    if (thread_num) {
        thread_pool_ = new ThreadPool(thread_num, max_event_size_);
        if (!thread_pool_->start()) {
             return false;
        }
    }
    return true;
}

EventSlot *EventModule::alloc_slot() {
    if (!free_list_) {
        return nullptr;
    }
    EventSlot *s = free_list_;
    free_list_ = s->free_list;
    s->free_list = nullptr;
    return s;
}

void EventModule::free_slot(EventSlot *s) {
    if (!s) {
        return;
    }
    s->event = nullptr;
    s->free_list = free_list_;
    free_list_ = s;
}

bool EventModule::add_event(NetEvent *e) {
    EventSlot *s = alloc_slot();
    if (!s) {
        return false;
    }
    s->event = e;
    e->slot_ = s;
    return true;
}

bool EventModule::del_event(NetEvent *e) {
    free_slot(e->slot_);
    e->slot_ = nullptr;
    return true;
}
