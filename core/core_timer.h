#ifndef CORE_TIMER_H
#define CORE_TIMER_H

#include <vector>
#include <memory>
#include <functional>
#include "def.h"
#include "core_mutex.h"
#include "core_singleton.h"


class TimerManager;
class Timer;

using TimerSPtr = std::shared_ptr<Timer>;
using TimerWPtr = std::weak_ptr<Timer>;


class Timer: public std::enable_shared_from_this<Timer>
{
    friend TimerManager;
public:
    Timer();
    virtual ~Timer();
    void start_timer(uint32 millisecond, bool loop);
    void stop_timer();
    uint64 get_timer_id() const;
    bool is_loop() const;
    bool is_cancel() const;
    void set_loop(bool loop);
protected:
    virtual void on_timer() = 0;
//private:
    uint64      id_;
    uint64      expire_,ex;
    uint32      interval_;
    bool        loop_:4;
    bool        cancel_:4;
};


class TimerTask: public Timer
{
public:
    using TimerFunc = std::function<void()>;
    explicit TimerTask(TimerFunc func);
    virtual ~TimerTask();
protected:
    virtual void on_timer();
private:
    TimerFunc func_;
};


class TimerManager
{
    friend Timer;
    struct TimerNode
    {
        TimerWPtr    timer;
        TimerNode   *next;
    };
    struct TimerQueue
    {
        TimerNode   *first;
        TimerNode   *last;
    };
    using TimeWheel = std::vector<TimerQueue>;
public:
    static TimerManager *instance();
    void expire_timer();
protected:
    TimerManager();
    ~TimerManager();
    bool add_timer(TimerSPtr timer_obj, uint32 millisecond, bool loop);
    void del_timer(TimerSPtr timer_obj);
private:
    void _tick(uint64 tk);
    SpinLock        spin_;
    uint64          last_tick_;
    uint64          next_timer_id_;
    uint32         *slot_;
    TimeWheel      *tw_;
};



// inline ==========================================================

inline uint64 Timer::get_timer_id() const {
    return id_;
}

inline bool Timer::is_loop() const {
    return loop_;
}

inline bool Timer::is_cancel() const {
    return cancel_;
}

inline void Timer::set_loop(bool loop) {
    loop_ = loop;
}

#endif // CORE_TIMER_H
