#ifndef TIMER_H
#define TIMER_H

#include <vector>
#include "def.h"
#include "core_mutex.h"


class TimerManager;

class Timer
{
    friend TimerManager;
public:
    Timer():id_(0), expire_(0), interval_(0), loop_(false), cancel_(false){};
    virtual ~Timer(){}
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

struct TimerNode
{
    Timer       *timer_;
    TimerNode   *next_;
};

struct TimerQueue
{
    TimerNode   *first_;
    TimerNode   *last_;
};

class TimerManager
{
    using TimeWheel = std::vector<TimerQueue>;
public:
    TimerManager();
    ~TimerManager();
    bool add_timer(Timer *timer, uint32 millisecond, bool loop);
    void del_timer(Timer *timer);
    void expire_timer();
private:
    void _tick(uint64 tk);
    SpinLock        spin_;
    uint64          last_tick_;
    uint64          next_timer_id_;
    uint32         *slot_;
    TimeWheel      *tw_;
};


#endif // TIMER_H
