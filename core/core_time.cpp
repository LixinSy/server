#include "core_time.h"
#include <cstring>

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
    struct tm gtm = *std::gmtime(&t);
    //mktime参数含义为localtime, 返回localtime对应的utc时间戳。
    //这里传入gmtime, 返回t的偏移时间戳
    time_t gt = std::mktime(&gtm);
    struct tm gtm2 = *std::localtime(&gt);
    int offset = ((t - gt ) + (gtm2.tm_isdst > 0 ? 3600 : 0));
    return offset;
}

int Time::timezone() {
    return Time::timeoffset() / Time::HOUR_S;
}

Time::Time() {
    timestamp_ = std::time(nullptr);
    tm_val_ = new struct tm();
    struct tm *tv = std::localtime(&timestamp_);
    memcpy((void*)tm_val_, (const void*)tv, sizeof (struct tm));
}

Time::Time(time_t timestump) {
    timestamp_ = timestump;
    tm_val_ = new struct tm();
    struct tm *tv = std::localtime(&timestamp_);
    memcpy((void*)tm_val_, (const void*)tv, sizeof (struct tm));
}

Time::Time(const Time &other) {
    timestamp_ = other.timestamp_;
    tm_val_ = new struct tm();
    memcpy((void*)tm_val_, (const void*)other.tm_val_, sizeof (struct tm));
}

Time::~Time() {
    delete tm_val_;
}

std::string Time::format(Time::Format format) const {
    std::string str = "";
    char tmpbuf[32] = { 0 };
    switch (format) {
    case Time::Format::YMDHMS_S:
        std::strftime(tmpbuf, sizeof (tmpbuf), "%Y-%m-%d %H:%M:%S", tm_val_);
        str = std::string(tmpbuf);
        break;
    case Time::Format::YMDHMS_N:
        std::strftime(tmpbuf, sizeof (tmpbuf), "%Y%m%d%H%M%S", tm_val_);
        str = std::string(tmpbuf);
        break;
    case Time::Format::YMD_S:
        std::strftime(tmpbuf, sizeof (tmpbuf), "%Y-%m-%d", tm_val_);
        str = std::string(tmpbuf);
        break;
    case Time::Format::YMD_N:
        std::strftime(tmpbuf, sizeof (tmpbuf), "%Y%m%d", tm_val_);
        str = std::string(tmpbuf);
        break;
    case Time::Format::HMS_S:
        std::strftime(tmpbuf, sizeof (tmpbuf), "%H:%M:%S", tm_val_);
        str = std::string(tmpbuf);
        break;
    case Time::Format::HMS_N:
        std::strftime(tmpbuf, sizeof (tmpbuf), "%H%M%S", tm_val_);
        str = std::string(tmpbuf);
        break;
    }
    return str;
}
