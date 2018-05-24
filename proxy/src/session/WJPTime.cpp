//
// Created by jipeng on 5/23/18.
//

#include "WJPTime.h"

Time::Time() : msec(linux_clock()) {}
Time::Time(int64_t when) : msec(when) {}
Time::Time(Time&& datime) : msec(datime.msec) {}
Time::Time(const Time& time)
{
    msec=time.msec;
}

Time& Time::operator=(const Time&time)
{
    msec=time.msec;
    return *this;
}

String Time::str()const
{
    char buf[64] = {0};
    time_t seconds = static_cast<time_t>(msec/1000);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    return buf;
}

#ifdef WJP_TEST
void unit_test_Time()
{
    Time a;
    Time b;
    b=a;
    b+=14000;
    std::cout<<"a: "<<a.str()<< "\nb: " << b.str()<< std::endl;
}
#endif
