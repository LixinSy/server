#ifndef CORE_MUTEX_H
#define CORE_MUTEX_H

#include <stdexcept>
#include <pthread.h>

class Mutex
{
public:
    Mutex(){
        pthread_mutex_init(&mtx_hander_, nullptr);
    }
    ~Mutex(){
        pthread_mutex_destroy(&mtx_hander_);
    }
    void lock(){
        int e = pthread_mutex_lock(&mtx_hander_);
        if (e)  throw std::runtime_error("pthread_mutex_lock error");
    }
    void unlock(){
        int e = pthread_mutex_unlock(&mtx_hander_);
        if (e)  throw std::runtime_error("pthread_mutex_unlock error");
    }
    bool try_lock() noexcept{
        return !pthread_mutex_trylock(&mtx_hander_);
    }
    pthread_mutex_t * get_mutex(){
        return &mtx_hander_;
    }
    //non-copyablle
    Mutex(const Mutex &) = delete ;
    Mutex & operator=(const Mutex &) = delete ;
private:
    pthread_mutex_t mtx_hander_;
};

class LockGuard
{
public:
    explicit LockGuard(Mutex &m):mtx_(m){
        mtx_.lock();
    }
    ~LockGuard(){
        mtx_.unlock();
    }
    //non-copyablle
    LockGuard(const LockGuard &) = delete ;
    LockGuard & operator=(const LockGuard &) = delete ;
private:
    Mutex &mtx_;
};

class SpinLock
{
public:
    SpinLock(){
        pthread_spin_init(&mtx_hander_, PTHREAD_PROCESS_SHARED);
    }
    ~SpinLock(){
        pthread_spin_destroy(&mtx_hander_);
    }
    void lock(){
        int e = pthread_spin_lock(&mtx_hander_);
        if (e)  throw std::runtime_error("pthread_spin_lock error");
    }
    void unlock(){
        int e = pthread_spin_unlock(&mtx_hander_);
        if (e)  throw std::runtime_error("pthread_spin_unlock error");
    }
    bool try_lock() noexcept{
        return !pthread_spin_trylock(&mtx_hander_);
    }
    pthread_spinlock_t * get_mutex(){
        return &mtx_hander_;
    }
    //non-copyablle
    SpinLock(const SpinLock &) = delete ;
    SpinLock & operator=(const SpinLock &) = delete ;
private:
    pthread_spinlock_t mtx_hander_;
};

#endif // CORE_MUTEX_H
