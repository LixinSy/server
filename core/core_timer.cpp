#include "core_timer.h"
#include "comm_time_util.h"
#include <stdio.h>

static const uint32 SI = 10;//ms
static const uint32 TIME_WHEEL_LEVEL = 4;
static const uint32 LEVEL_RADIX[TIME_WHEEL_LEVEL] = {10, 8, 6, 6};
static const uint32 LEVEL_SLOTS[TIME_WHEEL_LEVEL] = {1024, 256, 64, 64};
static const uint32 LEVEL_MASKS[TIME_WHEEL_LEVEL] = {1023, 255, 63, 63};
static const uint32 LEVEL_TICKS[TIME_WHEEL_LEVEL] = {1024, 1024*256, 1024*256*64, 1024*256*64*64};

TimerManager::TimerManager() {
    next_timer_id_ = 1;
    slot_ = new uint32[TIME_WHEEL_LEVEL];
    tw_ = new TimeWheel[TIME_WHEEL_LEVEL];
    for (uint32 i = 0; i < TIME_WHEEL_LEVEL; ++i) {
        slot_[i] = 0;
        tw_[i].resize(LEVEL_SLOTS[i], {nullptr, nullptr});
    }
}

TimerManager::~TimerManager() {
    delete [] slot_;
    delete [] tw_;
}

bool TimerManager::add_timer(Timer *timer, uint32 millisecond, bool loop) {
    TimerNode *new_node = new TimerNode;
    new_node->next_ = nullptr;
    new_node->timer_ = timer;
    uint32 level = 0;
    uint32 slot = 0;
    uint32 tick = millisecond / SI;
    if (tick == 0) tick = 1;
    for (uint32 i = 0; i < TIME_WHEEL_LEVEL; ++i) {
        tick += slot_[i];
        if (tick < LEVEL_SLOTS[i]) {
            level = i;
            slot = tick;
            break;
        }
        if (i == TIME_WHEEL_LEVEL - 1) {
            delete new_node;
            return false;
        }
        tick >>= LEVEL_RADIX[i];
        tick --;
    }
    printf("new timer [%u, %u], cur=%u\n", level, slot, slot_[0]);
    TimerQueue &queue = tw_[level][slot];
    if (queue.last_){
        queue.last_->next_ = new_node;
        queue.last_ = new_node;
    } else {
        queue.last_ = new_node;
        queue.first_ = queue.last_;
    }
    timer->id_ = next_timer_id_++;
    timer->expire_ = millisecond + TimeUtil::get_millisecond();
    timer->interval_ = millisecond;
    timer->loop_ = loop;
    return true;
}

void TimerManager::del_timer(Timer *timer) {
    uint32 level = 0;
    uint32 slot = 0;
    uint64 now_ms = TimeUtil::get_millisecond();
    uint32 tick = (timer->expire_ - now_ms) / SI;
    if (tick == 0) tick = 1;
    for (uint32 i = 0; i < TIME_WHEEL_LEVEL; ++i) {
        tick += slot_[i];
        if (tick < LEVEL_SLOTS[i]) {
            level = i;
            slot = tick;
            break;
        }
        if (i == TIME_WHEEL_LEVEL - 1) {
            return ;
        }
        tick >>= LEVEL_RADIX[i];
        tick --;
    }
    TimerQueue &queue = tw_[level][slot];
    TimerNode *node = queue.first_, *tmp_node = nullptr;
    if (node && node->timer_ == timer) {
        queue.first_ = node->next_;
        if (queue.first_ == nullptr) {
            queue.last_ = nullptr;
        }
    } else {
        for (; node; ) {
            if (node->next_->timer_ == timer) {
                tmp_node = node->next_;
                node->next_ = node->next_->next_;
                break;
            }
        }
    }
    if (tmp_node) {
        delete tmp_node;
    }
}

void TimerManager::expire_timer() {
    TimerQueue &queue = tw_[0][slot_[0]];
    TimerNode *node = queue.first_;// *tmp_node = nullptr;
    for (; node; node = queue.first_) {
        queue.first_ = queue.first_->next_;
        if (queue.first_ == nullptr) {
            queue.last_ = queue.first_;
        }
        node->next_ = nullptr;
        node->timer_->on_timer();
        if (node->timer_->loop_) {
            add_timer(node->timer_, node->timer_->interval_, true);
        }
        delete node;
    }
    uint64 now_ms = TimeUtil::get_millisecond();
    uint32 tick, slot, next_level;
    slot_[0] ++;
    //printf("level slot [%u, %u]\n", 0, slot_[0]);
    for (uint32 i = 0; i < TIME_WHEEL_LEVEL; ++i) {
        if (slot_[i] >= LEVEL_SLOTS[i]) {
            slot_[i] = 0;
            next_level = i+1;
            if (next_level == TIME_WHEEL_LEVEL) {
                break;
            }
            TimerQueue &next_level_queue = tw_[next_level][slot_[next_level]];
            for (node = next_level_queue.first_; node; node = next_level_queue.first_) {
                next_level_queue.first_ = next_level_queue.first_->next_;
                if (next_level_queue.first_ == nullptr) {
                    next_level_queue.last_ = next_level_queue.first_;
                }
                node->next_ = nullptr;
                tick = (node->timer_->expire_ - now_ms) / SI;
                if (tick == 0) tick = 1;
                slot = tick ;//& LEVEL_MASKS[i];
                TimerQueue &target_queue = tw_[i][slot];
                if (target_queue.last_){
                    target_queue.last_->next_ = node;
                    target_queue.last_ = node;
                } else {
                    target_queue.last_ = node;
                    target_queue.first_ = target_queue.last_;
                }
            }
            slot_[next_level] ++;
            //printf("level slot [%u, %u]\n", next_level, slot_[next_level]);
        }
    }
//    uint32 u=0;
//    for (uint32 i = 0; i < TIME_WHEEL_LEVEL; ++i)
//    {
//        for (uint32 j =0; j < tw_[i].size();j++){
//            for (TimerNode *n=tw_[i][j].first_; n ; n = n->next_) {
//                u++;
//            }
//        }
//    }
//    printf("total timer = %u\n", u);
}

