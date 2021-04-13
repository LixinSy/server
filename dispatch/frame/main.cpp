#include <bits/stdc++.h>
#include "def.h"
#include "comm_time_util.h"
#include "core_thread.h"
#include "core_thread_pool.h"
#include "core_timer.h"
#include "core_mutex.h"

using namespace std;
SpinLock sl;
void foo(void *d){
    do {
        sl.lock();
        uint64 i = *(uint64*)d;
        fprintf(stdout, "%lu hello %lu\n", i, pthread_self());
        sl.unlock();

    }while(1);
}

class TT: public Timer
{
public:
    TT(){
        set_loop(true);
    }
    virtual void on_timer() {
        printf("timer id = %lu, now = %lu\n", get_timer_id(), TimeUtil::get_millisecond());
    }
};


int main()
{
    printf("pid = %lu, __cplusplus = %ld\n", pthread_self(), __cplusplus);
    uint64 last = TimeUtil::get_millisecond();
    TimerManager m;
    TT t;
    m.add_timer(&t, 500000, true);
    while (1) {
        if (TimeUtil::get_millisecond() - last >= 10) {
            m.expire_timer();
            last = TimeUtil::get_millisecond();
        }
    }

    printf("%d\n", 0);
    return 0;
}
