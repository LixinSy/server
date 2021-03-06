#include "core_thread.h"
#include "comm_inc.h"


static void *start_thread_func(void *arg) {
    Runable *runable = (Runable*)(arg);
    runable->run();
    return nullptr;
}

Runable::Runable()
    : tid_(0)
{
}

Runable::~Runable() {

}

bool Runable::start() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int e = pthread_create(&tid_, nullptr, start_thread_func, this);
    pthread_attr_destroy(&attr);
    if (e) {
        return false;
    }
    return true;
}

bool Runable::sleep_for(uint64 msc) {
    if (pthread_self() == tid_) {
        ::usleep(msc*1000);
    }
    return true;
}

bool Runable::wait_for(uint64 msc) {
    if (pthread_self() == tid_) {
        ::usleep(msc*1000);
    }
    return true;
}

Thread::Thread(ThreadFunc func, const std::string& name)
    : func_(std::move(func)),
      name_(name)
{
}

Thread::~Thread() {

}

void Thread::run() {
    func_();
}


