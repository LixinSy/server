#include "core_thread_pool.h"

void *thread_pool_cycle(void *data){
    sigset_t sigset;
    sigfillset(&sigset);
    sigdelset(&sigset, SIGILL);
    sigdelset(&sigset, SIGFPE);
    sigdelset(&sigset, SIGSEGV);
    sigdelset(&sigset, SIGBUS);
    int err = pthread_sigmask(SIG_BLOCK, &sigset, NULL);
    if (err) {
        return nullptr;
    }
    ThreadPool *tp = (ThreadPool*)data;
    ThreadPoolTask *task = nullptr;
    fprintf(stderr, "tid = %lu\n", pthread_self());
    while (1) {
        tp->mtx_.lock();
        tp->cond_.wait(tp->mtx_, [tp]{return tp->task_queue_.first_;});
        task = tp->task_queue_.first_;
        tp->task_queue_.first_ = tp->task_queue_.first_->next_;
        if (tp->task_queue_.first_ == nullptr) {
            tp->task_queue_.last_ = nullptr;
        }
        tp->waiting_--;
        tp->mtx_.unlock();
        task->next_ = nullptr;
        task->func_(task->data_);
        delete task;
        task = nullptr;
    }
}

ThreadPool::ThreadPool(uint32 thread_num, uint32 max_task)
    : task_queue_({nullptr, nullptr}),
      waiting_(0),
      max_task_(max_task),
      nthread_(thread_num)
{
    int e;
    pthread_t tid;
    pthread_attr_t attr;
    e = pthread_attr_init(&attr);
    if (e)  throw std::runtime_error("ThreadPool::ThreadPool pthread_attr_init error");
    e = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (e)  throw std::runtime_error("ThreadPool::ThreadPool pthread_attr_setdetachstate error");
    for (uint32 i = 0; i < nthread_; ++i) {
        e = pthread_create(&tid, &attr, thread_pool_cycle, this);
        if (e)  throw std::runtime_error("ThreadPool::ThreadPool pthread_create error");
    }
    e = pthread_attr_destroy(&attr);
    if (e)  throw std::runtime_error("ThreadPool::ThreadPool pthread_attr_destroy error");
}

ThreadPool::~ThreadPool() {

}

int32 ThreadPool::add_task(FuncT func, void *arg) {
    ThreadPoolTask *new_task = new ThreadPoolTask;
    new_task->func_ = func;
    new_task->data_ = arg;
    new_task->next_ = nullptr;
    mtx_.lock();
    if (waiting_ >= max_task_) {
        mtx_.unlock();
        return -1;
    }
    if (task_queue_.last_) {
        task_queue_.last_->next_ = new_task;
        task_queue_.last_ = new_task;
    } else {
        task_queue_.last_ = new_task;
        task_queue_.first_ = task_queue_.last_;
    }
    waiting_++;
    cond_.notify_one();
    mtx_.unlock();
    return 0;
}


