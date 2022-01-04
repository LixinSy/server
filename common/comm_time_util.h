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

    //是否跨天某一小时, hour(0-23)
    static bool is_new_day_reset(time_t before, time_t after,  int resetHour) {
        before += Time::timeoffset();
        after += Time::timeoffset();
        return ((before - resetHour * Time::HOUR_S) / Time::ONE_DAY_S) != ((after - resetHour * Time::HOUR_S) / Time::ONE_DAY_S);
    }

    //获取下次重置时间（时刻）, nowSec为UTC时间戳
    static time_t GetNextResetTime(time_t nowSec, int resetHour) {
        time_t thisDayResetTime = nowSec - ((nowSec + Time::timeoffset()) % Time::ONE_DAY_S) + (int64(resetHour) * Time::HOUR_S);
        if (thisDayResetTime > nowSec) {
            return thisDayResetTime;
        } else {
            return thisDayResetTime + Time::ONE_DAY_S;
        }
    }

    //是否跨周before, after为UTC时间戳
    static bool IsWeekReset(time_t before, time_t after, int resetHour, int resetWeekDay) {
        time_t weekResetTime = TimeUtil::GetNextWeekResetTime(before, resetHour, resetWeekDay);
        return after > weekResetTime;
    }

    //获取下周重置时间（时刻）, nowSec为UTC时间戳
    static time_t GetNextWeekResetTime(time_t nowSec, int resetHour, int resetWeekDay) {
        Time nowTime(nowSec);
        int nowWeekDay = nowTime.wday();
        time_t diff = time_t(resetWeekDay-nowWeekDay) * Time::ONE_DAY_S;
        time_t thisWeekResetTime = nowSec - ((nowSec + Time::timeoffset()) % Time::ONE_DAY_S) + diff + (int64(resetHour) * Time::HOUR_S);
        if (thisWeekResetTime > nowSec) {
            return thisWeekResetTime;
        } else {
            return thisWeekResetTime + Time::ONE_WEEK_S;
        }
    }

};

#endif // COMM_TIME_UTIL_H
