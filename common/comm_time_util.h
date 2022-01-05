#ifndef COMM_TIME_UTIL_H
#define COMM_TIME_UTIL_H

#include "def.h"
#include <sys/time.h>
#include "core_time.h"

struct TimeUtil
{
    //获取毫秒
    static uint64 get_millisecond() {
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000ULL + tv.tv_usec / 1000ULL;
    }

    //是否跨天某一小时, resetHour(0-23)
    static bool is_new_day_reset(time_t before, time_t after,  int resetHour);

    //获取下次重置时间（时刻）, nowSec为UTC时间戳
    static time_t get_next_reset_time(time_t nowSec, int resetHour);

    //是否跨周before, after为UTC时间戳
    static bool is_week_reset(time_t before, time_t after, int resetHour, int resetWeekDay);

    //获取下周重置时间（时刻）, nowSec为UTC时间戳
    static time_t get_next_week_reset_time(time_t nowSec, int resetHour, int resetWeekDay);

    //获取某年某月的天数
    static int get_month_day_num(int year, int month);

};

#endif // COMM_TIME_UTIL_H
