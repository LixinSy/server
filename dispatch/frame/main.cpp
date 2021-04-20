#include <bits/stdc++.h>
#include "def.h"
#include "comm_time_util.h"
#include "core_thread.h"
#include "core_thread_pool.h"
#include "core_timer.h"
#include "core_mutex.h"

using namespace std;


class TT: public Timer
{
public:
    TT(){

    }
    virtual void on_timer() {
        uint64 t = TimeUtil::get_millisecond();
        printf("timer id = %lu, expire = %lu, now = %lu, rest= %ld\n",
               get_timer_id(), ex, t, t-ex);
    }
};
TT t;
void foo(void *d){
    //sleep(1);
    do {
        //m.add_timer((TT*)d, 2000, true);
    }while(0);
}

int main()
{
    srand(time(nullptr));
    printf("pid = %lu, __cplusplus = %ld\n", pthread_self(), __cplusplus);

    Thread th(std::bind(foo, &t), "");
    //th.start();
//    {
    TimerSPtr sp(new TT);
    sp->start_timer(2000, true);
//    }
    while (1) {
        TimerManager::instance()->expire_timer();
        //usleep(2500);
    }

    printf("%d\n", 0);
    return 0;
}
