#ifndef COMM_TIME_UTIL_H
#define COMM_TIME_UTIL_H

#include "def.h"
#include <sys/time.h>

#define ONE_HOUR_S 60*60
#define ONE_DAY_S 24*60*60

struct TimeUtil
{
    static uint64 get_millisecond(){
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000ULL + tv.tv_usec / 1000ULL;
    }

};

#endif // COMM_TIME_UTIL_H
