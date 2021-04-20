#ifndef CORE_THREADPOOL_H
#define CORE_THREADPOOL_H

#include <functional>
#include "def.h"
#include "core_mutex.h"
#include "core_cond.h"


struct ThreadPoolTask
{
    using TaskFunc = std::function<void(void *)>;
    TaskFunc             func;
    void                *data;
    ThreadPoolTask      *next;
};

class ThreadPool
{
    using FuncT = ThreadPoolTask::TaskFunc;
    friend void *thread_pool_cycle(void *data);
    struct TaskQueue
    {
        ThreadPoolTask *first;
        ThreadPoolTask *last;
    };

public:
    explicit ThreadPool(uint32 thread_num, uint32 max_task);
    ~ThreadPool();
    //non-copyablle
    ThreadPool(const ThreadPool &) = delete ;
    ThreadPool & operator=(const ThreadPool &) = delete ;

    int32 add_task(FuncT func, void *arg);
private:
    Mutex                mtx_;
    Condition            cond_;
    TaskQueue            task_queue_;
    uint32               waiting_;
    uint32               max_task_;
    uint32               nthread_;
};

#endif // CORE_THREADPOOL_H
