#include "core_timer.h"
#include "comm_time_util.h"
#include <stdio.h>

static const uint32 SI = 10;//ms
static const uint32 TIME_WHEEL_LEVEL = 4;
static const uint32 LEVEL_RADIX[TIME_WHEEL_LEVEL] = {4, 3, 3, 3};//{10, 8, 6, 6};
static const uint32 LEVEL_DIVIS[TIME_WHEEL_LEVEL] = {0, 4, 7, 10};//{10, 8, 6, 6};
static const uint32 LEVEL_SLOTS[TIME_WHEEL_LEVEL] = {16, 8, 8, 8};//{1024, 256, 64, 64};
static const uint32 LEVEL_MASKS[TIME_WHEEL_LEVEL] = {15, 7, 7, 7};//{1023, 255, 63, 63};
static const uint32 LEVEL_TICKS[TIME_WHEEL_LEVEL] = {16, 16*8, 16*8*8, 16*8*8*8};//{1024, 1024*256, 1024*256*64, 1024*256*64*64};

inline static int64_t get_tick() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 100ULL + tv.tv_usec / 10000ULL;
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
bool TimerManager::add_timer(Timer *timer, uint32 millisecond, bool loop) {
    timer->id_ = next_timer_id_++;
    timer->expire_ = (millisecond + TimeUtil::get_millisecond())/SI;
    timer->ex = millisecond + TimeUtil::get_millisecond();
    timer->interval_ = millisecond;
    timer->loop_ = loop;
    TimerNode *new_node = new TimerNode;
    new_node->next_ = nullptr;
    new_node->timer_ = timer;
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
    spin_.lock();
    TimerQueue &queue = tw_[level][slot];
    if (queue.last_){
        queue.last_->next_ = new_node;
        queue.last_ = new_node;
    } else {
        queue.last_ = new_node;
        queue.first_ = queue.last_;
    }
    spin_.unlock();
    return true;
}

void TimerManager::del_timer(Timer *timer) {
    timer->cancel_ = true;
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

void TimerManager::_tick(uint64 tk) {
    ++slot_[0];
    slot_[0] = slot_[0] & LEVEL_MASKS[0];
    TimerNode *node = nullptr;
    uint32 tick, level, slot, pre_level;
    for (uint32 i = 1; i < TIME_WHEEL_LEVEL; ++i) {
        pre_level = i-1;
        if (slot_[pre_level]) {
            break;
        }
        ++slot_[i];
        slot_[i] = slot_[i] & LEVEL_MASKS[i];
        //move timer
        //spin_.lock();
        TimerQueue &cur_queue = tw_[i][slot_[i]];
        for (node = cur_queue.first_; node; node = cur_queue.first_) {
            cur_queue.first_ = cur_queue.first_->next_;
            if (cur_queue.first_ == nullptr) {
                cur_queue.last_ = cur_queue.first_;
            }
            node->next_ = nullptr;
            if (!node->timer_) {
                delete node;
                continue;
            }
            if (node->timer_->expire_ < tk) {
                tick = 0;
            } else {
                tick = node->timer_->expire_ - tk;
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
            if (&target_queue == &cur_queue) {
                printf("circle!!!!!!!!!!!!!!!!!!!!!!!\n");
            }
            if (target_queue.last_){
                target_queue.last_->next_ = node;
                target_queue.last_ = node;
            } else {
                target_queue.last_ = node;
                target_queue.first_ = target_queue.last_;
            }
        }
        //spin_.unlock();
    }
    //spin_.lock();
    TimerQueue &queue = tw_[0][slot_[0]];
    for (node = queue.first_; node; node = queue.first_) {
        queue.first_ = queue.first_->next_;
        if (queue.first_ == nullptr) {
            queue.last_ = queue.first_;
        }
        node->next_ = nullptr;
        if (!node->timer_) {
            delete node;
            continue;
        }
        if (node->timer_->cancel_) {
            delete node;
            continue;
        }
        node->timer_->on_timer();
        if (node->timer_->loop_) {
            add_timer(node->timer_, node->timer_->interval_, true);
        }
        delete node;
    }
    //spin_.unlock();
}

