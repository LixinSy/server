#ifndef CORE_MUTEX_H
#define CORE_MUTEX_H

#include <stdexcept>
#include <pthread.h>

class Mutex
{
public:
    Mutex(bool recursive);
    ~Mutex();
    //non-copyablle
    Mutex(const Mutex &) = delete ;
    Mutex & operator=(const Mutex &) = delete ;

    void lock();
    void unlock();
    bool try_lock() noexcept;
    pthread_mutex_t * get_mutex() { return &mtx_hander_; }
    bool is_recursive() { return recursive_; }
private:
    pthread_mutex_t mtx_hander_;
    bool recursive_;
};

class SpinLock
{
public:
    SpinLock();
    ~SpinLock();
    //non-copyablle
    SpinLock(const SpinLock &) = delete ;
    SpinLock & operator=(const SpinLock &) = delete ;

    void lock();
    void unlock();
    bool try_lock() noexcept;
    pthread_spinlock_t * get_mutex() { return &mtx_hander_; }
private:
    pthread_spinlock_t mtx_hander_;
};


template<typename mutex_type>
class LockGuard
{
public:
    explicit LockGuard(mutex_type &m):mtx_(m){
        mtx_.lock();
    }
    ~LockGuard(){
        mtx_.unlock();
    }
    //non-copyablle
    LockGuard(const LockGuard &) = delete ;
    LockGuard & operator=(const LockGuard &) = delete ;
private:
    mutex_type &mtx_;
};


#endif // CORE_MUTEX_H
