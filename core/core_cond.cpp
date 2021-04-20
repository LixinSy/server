#include "core_cond.h"


Condition::Condition()
{
    int e = pthread_cond_init(&cond_, nullptr);
    if (e)  throw std::runtime_error("Condition::Condition pthread_cond_init error.");
}

Condition::~Condition() {
    pthread_cond_destroy(&cond_);
}

void Condition::notify_one() {
    pthread_cond_signal(&cond_);
}

void Condition::notify_all() {
    pthread_cond_broadcast(&cond_);
}

void Condition::wait(Mutex &mtx) {
    pthread_cond_wait(&cond_, mtx.get_mutex());
}

void Condition::wait(Mutex &mtx, std::function<bool()> pred) {
    while (!pred())
        wait(mtx);
}
