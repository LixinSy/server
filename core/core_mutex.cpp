#include "core_mutex.h"

Mutex::Mutex(bool recursive)
    : recursive_(recursive)
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

Mutex::~Mutex() {
    pthread_mutex_destroy(&mtx_hander_);
}

void Mutex::lock() {
    pthread_mutex_lock(&mtx_hander_);
}

void Mutex::unlock() {
    pthread_mutex_unlock(&mtx_hander_);
}

bool Mutex::try_lock() noexcept {
    return !pthread_mutex_trylock(&mtx_hander_);
}


SpinLock::SpinLock() {
    int e = pthread_spin_init(&mtx_hander_, PTHREAD_PROCESS_PRIVATE);
    if (e) {
        throw std::runtime_error("pthread_spin_init error");
    }
}

SpinLock::~SpinLock() {
    pthread_spin_destroy(&mtx_hander_);
}

void SpinLock::lock() {
    pthread_spin_lock(&mtx_hander_);
}

void SpinLock::unlock() {
    pthread_spin_unlock(&mtx_hander_);
}

bool SpinLock::try_lock() noexcept {
    return !pthread_spin_trylock(&mtx_hander_);
}

