#include "net_event.h"
#include "net_event_module.h"

NetEvent::NetEvent(EventModule *m)
    : fd_(0),
      ref_(1),
      state_(State::NEW),
      regist_events_(0),
      active_events_(0),
      slot_(nullptr),
      event_module_(m)
{

}

void NetEvent::attach_slot(EventSlot *s) {
    slot_ = s;
    s->event = this;
}

void NetEvent::detach_slot(EventSlot *s) {
    slot_ = nullptr;
    s->event = nullptr;
}

void NetEvent::release() {
    if (--ref_ <= 0) {
        if (slot_) {
            event_module_->free_slot(slot_);
        }
        delete this;
    }
}
