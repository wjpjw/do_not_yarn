//
// Created by jipeng on 5/23/18.
//

#ifndef DATAACCESSPROXY_TIME_H
#define DATAACCESSPROXY_TIME_H

#include "wjp.h"
#include <boost/operators.hpp>

/* resolution: Millisecond */

class Time :    boost::addable<Time,
        boost::addable<Time,int64_t,
                boost::subtractable<Time,
                        boost::subtractable<Time,int64_t ,
                                boost::less_than_comparable<Time,
                                        boost::less_than_comparable<Time,int64_t ,
                                                boost::equality_comparable<Time,
                                                        boost::equality_comparable<Time,int64_t > > > > > > > >
{
public:
    Time();                         // by default time is now
    Time(int64_t when);
    Time(Time&& datime);

    Time& operator=(const Time& time);
    Time(const Time& time);

    inline int64_t val()const {return msec;}
    inline int64_t elapsed()const {return linux_clock()-msec;}
    String str()const;
    friend bool operator<(const Time& lhs, const Time& rhs)
    {
        return lhs.val() < rhs.val();
    }
    friend bool operator==(const Time& lhs, const Time& rhs)
    {
        return lhs.val() == rhs.val();
    }


    /* int64_t */
    friend bool operator==(const Time& lhs, int64_t rhs)
    {
        return lhs.val() == rhs;
    }
    friend bool operator<(const Time& lhs, int64_t rhs)
    {
        return lhs.val() < rhs;
    }
    Time& operator+=(int64_t datime) {
        msec+=datime;
        return *this;
    }
    Time& operator-=(int64_t datime) {
        msec-=datime;
        return *this;
    }


    Time& operator+=(const Time& datime) {
        msec+=datime.msec;
        return *this;
    }
    Time& operator-=(const Time& datime) {
        msec-=datime.msec;
        return *this;
    }
private:
    int64_t msec;  // milli seconds since 1970-0-0
};



#ifdef WJP_TEST
extern void unit_test_Time();
#endif

#endif //DATAACCESSPROXY_TIME_H
