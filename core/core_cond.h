#ifndef CORE_COND_H
#define CORE_COND_H

#include <functional>
#include <pthread.h>
#include "core_mutex.h"


class Condition
{
public:
    Condition();
    ~Condition();
    //non-copyable
    Condition(const Condition&) = delete;
    Condition& operator=(const Condition&) = delete;

    void notify_one();
    void notify_all();
    void wait(Mutex &mtx);
    void wait(Mutex &mtx, std::function<bool()> pred);
private:
    pthread_cond_t cond_;
};

#endif // CORE_COND_H
