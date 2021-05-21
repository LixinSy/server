#include "net_reactor.h"


Reactor::Reactor(EventModule *m)
    : event_module_(m),
      thread_pool_(nullptr)
{

}
Reactor::~Reactor() {
    if (thread_pool_) {
        delete thread_pool_;
    }
    delete event_module_;
}
