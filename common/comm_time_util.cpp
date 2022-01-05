#include "comm_time_util.h"
#include "core_time.h"

//是否跨天某一小时, resetHour(0-23)
bool TimeUtil::is_new_day_reset(time_t before, time_t after,  int resetHour) {
    before += Time::timeoffset();
    after += Time::timeoffset();
    return ((before - resetHour * Time::HOUR_S) / Time::ONE_DAY_S) != ((after - resetHour * Time::HOUR_S) / Time::ONE_DAY_S);
}

//获取下次重置时间（时刻）, nowSec为UTC时间戳
time_t TimeUtil::get_next_reset_time(time_t nowSec, int resetHour) {
    time_t thisDayResetTime = nowSec - ((nowSec + Time::timeoffset()) % Time::ONE_DAY_S) + (int64(resetHour) * Time::HOUR_S);
    if (thisDayResetTime > nowSec) {
        return thisDayResetTime;
    } else {
        return thisDayResetTime + Time::ONE_DAY_S;
    }
}

//是否跨周before, after为UTC时间戳
bool TimeUtil::is_week_reset(time_t before, time_t after, int resetHour, int resetWeekDay) {
    time_t weekResetTime = TimeUtil::get_next_week_reset_time(before, resetHour, resetWeekDay);
    return after > weekResetTime;
}

//获取下周重置时间（时刻）, nowSec为UTC时间戳
time_t TimeUtil::get_next_week_reset_time(time_t nowSec, int resetHour, int resetWeekDay) {
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

//获取某年某月的天数
int TimeUtil::get_month_day_num(int year, int month)  {
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        return 31;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        bool leapYear = (year%4 == 0 && year%100 != 0) || year%400 == 0;
        if (leapYear) {
            return 29;
        } else {
            return 28;
        }
    }
}
