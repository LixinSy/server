#ifndef CORE_MUTEX_H
#define CORE_MUTEX_H

#include <stdexcept>
#include <pthread.h>

class Mutex
{
public:
    Mutex(bool recursive)
        :recursive_(recursive)
    {
        if (recursive) {
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            int e = pthread_mutex_init(&mtx_hander_, &attr);
            pthread_mutexattr_destroy(&attr);
            if (e) {
                throw std::runtime_error("pthread_mutex_init error");
            }
        } else {
            int e = pthread_mutex_init(&mtx_hander_, nullptr);
            if (e) {
                throw std::runtime_error("pthread_mutex_init error");
            }
        }
    }
    ~Mutex() {
        pthread_mutex_destroy(&mtx_hander_);
    }
    void lock() {
        pthread_mutex_lock(&mtx_hander_);
    }
    void unlock() {
        pthread_mutex_unlock(&mtx_hander_);
    }
    bool try_lock() noexcept {
        return !pthread_mutex_trylock(&mtx_hander_);
    }
    pthread_mutex_t * get_mutex() {
        return &mtx_hander_;
    }
    bool is_recursive() {
        return recursive_;
    }
    //non-copyablle
    Mutex(const Mutex &) = delete ;
    Mutex & operator=(const Mutex &) = delete ;
private:
    pthread_mutex_t mtx_hander_;
    bool recursive_;
};

class SpinLock
{
public:
    SpinLock(){
        int e = pthread_spin_init(&mtx_hander_, PTHREAD_PROCESS_PRIVATE);
        if (e) {
            throw std::runtime_error("pthread_spin_init error");
        }
    }
    ~SpinLock(){
        pthread_spin_destroy(&mtx_hander_);
    }
    void lock(){
        pthread_spin_lock(&mtx_hander_);
    }
    void unlock(){
        pthread_spin_unlock(&mtx_hander_);
    }
    bool try_lock() noexcept {
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
