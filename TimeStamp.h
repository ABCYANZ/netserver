#pragma once
#include <time.h>
class TimeStamp
{
private:
    time_t Stamp_;//整数表示的时间（从1970到现在已逝去的秒数）。
public:
    TimeStamp();

    void updateStamp();//更新时间
    time_t Stamp();//返回最后活跃时间
    time_t now();//返回当前时间
    ~ TimeStamp();
};