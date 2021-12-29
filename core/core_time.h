#ifndef TIME_H
#define TIME_H

#include <string>
#include <ctime>
#include "def.h"


class Time final
{
public:
    enum  {
        SECOND = 1,
        MINUTE_S = 60,
        HOUR_S = 60 * MINUTE_S,
        ONE_DAY_S = 24 * HOUR_S,
        ONE_WEEK_S = 7 * ONE_DAY_S,
    };
    enum Format {
        YMDHMS_S = 0,   //2006-01-02 15:04:05
        YMDHMS_N,       //20060102150405
        YMD_S,          //2006-01-02
        YMD_N,          //20060102
        HMS_S,          //15:04:05
        HMS_N,          //150405
    };
    enum Week {
        SUNDAY = 0,
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
    };
    enum Month {
        JANUARY = 1,
        FEBRUARY,
        MARCH,
        APRIL,
        MAY,
        JUNE,
        JULY,
        AUGUST,
        SEPTEMBER,
        OCTOBER,
        NOVEMBER,
        DECEMBER,
    };
public:
    static Time parser(std::string date_time);
    static Time now();
    //返回当前UTC时间戳
    static time_t utc();
    static int timezone();
    static int timeoffset();
public:
    Time();
    Time(time_t timestump);
    Time(const Time &other);
    ~Time();

    int hour() const { return tm_val_->tm_hour; }
    int minute() const { return tm_val_->tm_min; }
    int second() const { return tm_val_->tm_sec; }
    int year() const { return tm_val_->tm_year+1900; }
    int month() const { return tm_val_->tm_mon+1; }
    int mday() const { return tm_val_->tm_mday; }
    int wday() const { return tm_val_->tm_wday; }
    int yday() const { return tm_val_->tm_yday; }
    int isdist() const { return tm_val_->tm_isdst; }

    time_t timestamp() const { return timestamp_; }
    std::string format(Time::Format format) const;
private:
    time_t timestamp_;      //UTC时间戳
    struct tm *tm_val_;
};

#endif // TIME_H
