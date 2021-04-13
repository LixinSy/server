#include "core_thread.h"

static void *start_thread_func(void *arg) {
    Runable *runable = (Runable*)(arg);
    runable->run();
    return nullptr;
}

Runable::Runable()
    :tid_(0)
{
}
Runable::~Runable() {
    pthread_detach(tid_);
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
    //pthread_join(tid_, nullptr);
    //pthread_detach(tid_);
    return true;
}

Thread::Thread(ThreadFunc func, const std::string& name)
    :func_(std::move(func)), name_(name)
{
}
Thread::~Thread() {

}
void Thread::run() {
    func_();
}


