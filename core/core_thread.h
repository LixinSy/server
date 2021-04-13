#ifndef CORE_THPOOL_H
#define CORE_THPOOL_H

#include <string>
#include <functional>
#include <pthread.h>

class Runable
{
public:
    Runable();
    virtual ~Runable();
    virtual void run()=0;
    bool start();
    // non-copyable
    Runable(const Runable &) = delete;
    Runable & operator=(const Runable &) = delete;
protected:
    pthread_t       tid_;
};

class Thread: public Runable
{
public:
    using ThreadFunc = std::function<void()>;
    explicit Thread(ThreadFunc func, const std::string& name);
    ~Thread();
    virtual void run();
    const std::string& get_name() const { return name_; }
    // non-copyable
    Thread(const Thread &) = delete;
    Thread & operator=(const Thread &) = delete;
private:
    ThreadFunc      func_;
    std::string     name_;
};




#endif // CORE_THPOOL_H
