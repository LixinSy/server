#ifndef CORE_THREADPOOL_H
#define CORE_THREADPOOL_H

#include <functional>
#include "def.h"
#include "core_mutex.h"
#include "core_cond.h"


class ThreadPool final
{
public:
    using TaskFunc = std::function<void(void *)>;
private:
    friend void *thread_pool_cycle(void *data);
    struct ThreadPoolTask
    {
        TaskFunc             func;
        void                *data;
        ThreadPoolTask      *next;
    };
    struct TaskQueue
    {
        ThreadPoolTask      *first;
        ThreadPoolTask      *last;
    };

public:
    explicit ThreadPool(uint32 thread_num, uint32 max_task);
    ~ThreadPool();
    //non-copyablle
    ThreadPool(const ThreadPool &) = delete ;
    ThreadPool & operator=(const ThreadPool &) = delete ;
    bool start();
    int add_task(TaskFunc func, void *arg);
private:
    Mutex               mtx_;
    Condition           cond_;
    TaskQueue           task_queue_;
    uint32              waiting_;
    uint32              max_task_;
    uint32              nthread_;
};

#endif // CORE_THREADPOOL_H
