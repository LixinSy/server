#ifndef TIME_H
#define TIME_H

#include <string>
#include <ctime>
#include "def.h"


enum class TimeFormat {
    YMDHMS_S = 0,   //2006-01-02 15:04:05
    YMDHMS_N,       //20060102150405
    YMD_S,          //2006-01-02
    YMD_N,          //20060102
    HMS_S,          //15:04:05
    HMS_N,          //150405
};

enum class Week {
    SUNDAY = 0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
};

enum class Month {
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

class Time final
{
public:
    static const int SECOND = 1;
    static const int MINUTE_S = 60;
    static const int HOUR_S = 60 * MINUTE_S;
    static const int ONE_DAY_S = 24 * HOUR_S;
    static const int ONE_WEEK_S = 7 * ONE_DAY_S;
    static Time parser(std::string date_time);
    static Time now();
    //返回当前UTC时间戳
    static time_t utc();
    static int timezone();
    static int timeoffset();
public:
    Time();
    Time(time_t timestump);
    Time(int year, int month, int day, int hour, int minute, int second);
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
    std::string format(TimeFormat format) const;

    Time& operator += (const time_t& t);
    Time& operator -= (const time_t& t);
    time_t operator - (const Time &other) const;
    bool operator < (const Time &other) const;
    bool operator <= (const Time &other) const;
    bool operator > (const Time &other) const;
    bool operator >= (const Time &other) const;
    bool operator == (const Time &other) const;
    bool operator != (const Time &other) const;
private:
    time_t timestamp_;      //UTC时间戳
    struct tm *tm_val_;
};

void get_localtime(const time_t *t, struct tm *tmout);
void get_gmtime(const time_t *t, struct tm *tmout);

#endif // TIME_H
