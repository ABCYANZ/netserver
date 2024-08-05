#include"TimeStamp.h"

TimeStamp::TimeStamp()
{
    time(&Stamp_);
}

void TimeStamp::updateStamp()
{
    time(&Stamp_);
}

time_t TimeStamp::Stamp()
{
    return Stamp_;
}

time_t TimeStamp::now()
{
    time_t t;
    time(&t);
    return t;
}

TimeStamp::~TimeStamp()
{
    
}

