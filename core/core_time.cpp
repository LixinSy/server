#include "core_time.h"
#include <cstring>

void get_localtime(const time_t *t, struct tm *tmout) {
#if __POSIX__
    ::localtime_r(t, tmout);
#elif __WINDOWS__
    ::localtime_s(tmout, t);
#else
    struct tm *tv = std::localtime(t);
    memcpy((void*)tmout, (const void*)tv, sizeof (struct tm));
#endif
}

void get_gmtime(const time_t *t, struct tm *tmout) {
#if __POSIX__
    ::gmtime_r(t, tmout);
#elif __WINDOWS__
    ::gmtime_s(tmout, t);
#else
    struct tm *tv = std::gmtime(t);
    memcpy((void*)tmout, (const void*)tv, sizeof (struct tm));
#endif
}

Time Time::parser(std::string date_time) {
    Time date;
    std::string format;
    if (date_time[4] == '-') {
        format = "%Y-%m-%d %H:%M:%S";
    } else if (date_time[4] == '/') {
        format = "%Y/%m/%d %H:%M:%S";
    } else {
        throw "date time format invalid";
    }
    ::strptime(date_time.data(), format.data(), date.tm_val_);
    date.tm_val_->tm_isdst = -1;
    date.timestamp_  = mktime(date.tm_val_);
    return date;
}

Time Time::now() {
    Time date;
    return date;
}

time_t Time::utc() {
    return std::time(nullptr);
}

int Time::timeoffset() {
    time_t t = std::time(NULL);
    //获取utc时间
    struct tm gtm;
    get_gmtime(&t, &gtm);
    //mktime参数含义为localtime, 返回localtime对应的utc时间戳。
    //这里传入gmtime, 返回t的偏移时间戳
    time_t gt = std::mktime(&gtm);
    struct tm gtm2;
    get_localtime(&gt, &gtm2);
    int offset = ((t - gt ) + (gtm2.tm_isdst > 0 ? 3600 : 0));
    return offset;
}

int Time::timezone() {
    return Time::timeoffset() / Time::HOUR_S;
}

Time::Time() {
    timestamp_ = std::time(nullptr);
    tm_val_ = new struct tm();
    get_localtime(&timestamp_, tm_val_);
}

Time::Time(time_t timestump) {
    timestamp_ = timestump;
    tm_val_ = new struct tm();
    get_localtime(&timestamp_, tm_val_);
}

Time::Time(const Time &other) {
    timestamp_ = other.timestamp_;
    tm_val_ = new struct tm();
    memcpy((void*)tm_val_, (const void*)other.tm_val_, sizeof (struct tm));
}

Time::Time(int year, int month, int day, int hour, int minute, int second) {
    tm_val_ = new struct tm();
    tm_val_->tm_year = year-1900;
    tm_val_->tm_mon = month-1;
    tm_val_->tm_mday = day;
    tm_val_->tm_hour = hour;
    tm_val_->tm_min = minute;
    tm_val_->tm_sec = second;
    timestamp_ = std::mktime(tm_val_);
}

Time::~Time() {
    delete tm_val_;
}

std::string Time::format(TimeFormat format) const {
    std::string str;
    size_t n = 0;
    str.resize(32);
    switch (format) {
    case TimeFormat::YMDHMS_S:
        n = std::strftime(&str[0], str.capacity(), "%Y-%m-%d %H:%M:%S", tm_val_);
        break;
    case TimeFormat::YMDHMS_N:
        n = std::strftime(&str[0], str.capacity(), "%Y%m%d%H%M%S", tm_val_);
        break;
    case TimeFormat::YMD_S:
        n = std::strftime(&str[0], str.capacity(), "%Y-%m-%d", tm_val_);
        break;
    case TimeFormat::YMD_N:
        n = std::strftime(&str[0], str.capacity(), "%Y%m%d", tm_val_);
        break;
    case TimeFormat::HMS_S:
        n = std::strftime(&str[0], str.capacity(), "%H:%M:%S", tm_val_);
        break;
    case TimeFormat::HMS_N:
        n = std::strftime(&str[0], str.capacity(), "%H%M%S", tm_val_);
        break;
    }
    str.resize(n);
    return str;
}

Time& Time::operator += (const time_t& t) {
    timestamp_ += t;
    get_localtime(&timestamp_, tm_val_);
    return *this;
}

Time& Time::operator -= (const time_t& t) {
    timestamp_ -= t;
    get_localtime(&timestamp_, tm_val_);
    return *this;
}

time_t Time::operator - (const Time &other) const {
    return timestamp_ - other.timestamp_;
}

bool Time::operator < (const Time &other) const {
    return timestamp_ < other.timestamp_;
}

bool Time::operator <= (const Time &other) const {
    return timestamp_ <= other.timestamp_;
}

bool Time::operator > (const Time &other) const {
    return timestamp_ > other.timestamp_;
}

bool Time::operator >= (const Time &other) const {
    return timestamp_ >= other.timestamp_;
}

bool Time::operator == (const Time &other) const {
    return timestamp_ == other.timestamp_;
}

bool Time::operator != (const Time &other) const {
    return timestamp_ != other.timestamp_;
}
