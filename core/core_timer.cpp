#include "core_timer.h"
#include <stdio.h>
#include "comm_time_util.h"


static const uint32 SI = 10;//ms
static const uint32 TIME_WHEEL_LEVEL = 4;
static const uint32 LEVEL_DIVIS[TIME_WHEEL_LEVEL] = /*{0, 4, 7, 10};//*/{0, 10, 18, 24};
static const uint32 LEVEL_SLOTS[TIME_WHEEL_LEVEL] = /*{16, 8, 8, 8};//*/{1024, 256, 64, 64};
static const uint32 LEVEL_MASKS[TIME_WHEEL_LEVEL] = /*{15, 7, 7, 7};//*/{1023, 255, 63, 63};
static const uint32 LEVEL_TICKS[TIME_WHEEL_LEVEL] = /*{16, 16*8, 16*8*8, 16*8*8*8};//*/{1024, 1024*256, 1024*256*64, 1024*256*64*64};

inline static int64_t get_tick() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 100ULL + tv.tv_usec / 10000ULL;
}


Timer::Timer()
    : id_(0),
      expire_(0),
      interval_(0),
      loop_(false),
      cancel_(false)
{
}

Timer::~Timer() {

}

void Timer::start_timer(uint32 millisecond, bool loop) {
    TimerManager::instance()->add_timer(shared_from_this(), millisecond, loop);
}

void Timer::stop_timer() {
    cancel_ = false;
}


TimerTask::TimerTask(TimerFunc func)
    : func_(std::move(func))
{
}

TimerTask::~TimerTask() {

}

void TimerTask::on_timer() {
    func_();
}


TimerManager::TimerManager() {
    next_timer_id_ = 1;
    slot_ = new uint32[TIME_WHEEL_LEVEL];
    tw_ = new TimeWheel[TIME_WHEEL_LEVEL];
    for (uint32 i = 0; i < TIME_WHEEL_LEVEL; ++i) {
        slot_[i] = 0;
        tw_[i].resize(LEVEL_SLOTS[i], {nullptr, nullptr});
    }
    last_tick_ = get_tick();
}

TimerManager::~TimerManager() {
    delete [] slot_;
    delete [] tw_;
}
#include <string>
#include <sstream>
std::string vec_as_string(uint32 *v, uint32 n) {
    std::ostringstream os;
    os<<"[";
    for (uint32 i=0; i<n; i++){
        os<<v[i]<<",";
    }
    os<<"]";
    return os.str();
}

TimerManager *TimerManager::instance() {
    static TimerManager obj;
    return &obj;
}

void TimerManager::expire_timer() {
    uint64 now_tk = get_tick();
    if (now_tk > last_tick_) {
        for (uint64 tk = last_tick_+1; tk <= now_tk; ++tk) {
            _tick(tk);
        }
        last_tick_ = now_tk;
    } else if (now_tk < last_tick_){
        last_tick_ = now_tk;
    }
}

bool TimerManager::add_timer(TimerPtr timer, uint32 millisecond, bool loop) {
    timer->id_ = next_timer_id_++;
    timer->expire_ = (millisecond + TimeUtil::get_millisecond())/SI;
    timer->interval_ = millisecond;
    timer->loop_ = loop;
    TimerNode *new_node = new TimerNode;
    new_node->next = nullptr;
    new_node->timer = timer;
    uint32 level, slot=0;
    uint32 tick = millisecond / SI;
    uint32 tmp = tick;
    if (tick == 0) tick = 1;
    for (level = 0; level < TIME_WHEEL_LEVEL; ++level) {
        if (tick < LEVEL_TICKS[level]) {
            tick >>= LEVEL_DIVIS[level];
            slot = (tick + slot_[level]) & LEVEL_MASKS[level];
            break;
        }
    }
    if (level == TIME_WHEEL_LEVEL) {
        --level;
        slot = LEVEL_MASKS[level];
    }
    printf("------------new timer [%u, %u], tick=%u, cur=%s\n", level, slot, tmp, vec_as_string(slot_, 4).c_str());
    bool succeed = spin_.try_lock();
    TimerQueue &queue = tw_[level][slot];
    if (queue.last){
        queue.last->next = new_node;
        queue.last = new_node;
    } else {
        queue.last = new_node;
        queue.first = queue.last;
    }
    if (succeed) {
        spin_.unlock();
    }
    return true;
}

void TimerManager::del_timer(TimerPtr timer) {
    timer->cancel_ = true;
}

void TimerManager::_tick(uint64 tk) {
    ++slot_[0];
    slot_[0] = slot_[0] & LEVEL_MASKS[0];
    TimerNode *node = nullptr;
    TimerPtr timer = nullptr;
    uint32 tick, level, slot;
    for (uint32 i = 1; i < TIME_WHEEL_LEVEL; ++i) {
        if (slot_[i-1]) {
            break;
        }
        ++slot_[i];
        slot_[i] = slot_[i] & LEVEL_MASKS[i];
        //move timer
        spin_.lock();
        TimerQueue &cur_queue = tw_[i][slot_[i]];
        for (node = cur_queue.first; node; node = cur_queue.first) {
            cur_queue.first = cur_queue.first->next;
            if (cur_queue.first == nullptr) {
                cur_queue.last = cur_queue.first;
            }
            node->next = nullptr;
            timer = node->timer.lock();
            if (!timer) {
                delete node;
                continue;
            }
            if (timer->expire_ < tk) {
                tick = 0;
            } else {
                tick = timer->expire_ - tk;
            }
            uint32 tmp=tick;
            // re-get level, slot
            for (level = 0; level < TIME_WHEEL_LEVEL; ++level) {
                if (tick < LEVEL_TICKS[level]) {
                    tick >>= LEVEL_DIVIS[level];
                    slot = (tick + slot_[level]) & LEVEL_MASKS[level];
                    break;
                }
            }
            if (level == TIME_WHEEL_LEVEL) {
                --level;
                slot = LEVEL_MASKS[level];
            }
            printf("------------move timer [%u, %u][%u, %u], tick = %u, cur=%s\n", i, slot_[i], level, slot, tmp, vec_as_string(slot_, 4).c_str());
            // put timer
            TimerQueue &target_queue = tw_[level][slot];
            //if (&target_queue == &cur_queue) {
            //    printf("circle!!!!!!!!!!!!!!!!!!!!!!!\n");
            //}
            if (target_queue.last){
                target_queue.last->next = node;
                target_queue.last = node;
            } else {
                target_queue.last = node;
                target_queue.first = target_queue.last;
            }
        }
        spin_.unlock();
    }
    spin_.lock();
    TimerQueue &queue = tw_[0][slot_[0]];
    for (node = queue.first; node; node = queue.first) {
        queue.first = queue.first->next;
        if (queue.first == nullptr) {
            queue.last = queue.first;
        }
        node->next = nullptr;
        timer = node->timer.lock();
        if (!timer || timer->cancel_) {
            delete node;
            continue;
        }
        timer->on_timer();
        if (timer->loop_) {
            add_timer(timer, timer->interval_, true);
        }
        delete node;
    }
    spin_.unlock();
}

