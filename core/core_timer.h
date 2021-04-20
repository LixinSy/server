#ifndef CORE_TIMER_H
#define CORE_TIMER_H

#include <vector>
#include <memory>
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
    Timer():id_(0), expire_(0), interval_(0), loop_(false), cancel_(false){};
    virtual ~Timer(){}
    void start_timer(uint32 millisecond, bool loop);
    void stop_timer();
    uint64 get_timer_id(){  return id_; }
    bool is_loop(){ return loop_; }
    bool is_cancel() {return cancel_; }
    void set_loop(bool loop) {loop_ = loop; }
protected:
    virtual void on_timer() = 0;
//private:
    uint64      id_;
    uint64      expire_,ex;
    uint32      interval_;
    bool        loop_:4;
    bool        cancel_:4;
};


class TimerManager
{
    friend Timer;
    struct TimerNode
    {
        TimerWPtr    timer_;
        TimerNode   *next_;
    };
    struct TimerQueue
    {
        TimerNode   *first_;
        TimerNode   *last_;
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


#endif // CORE_TIMER_H
