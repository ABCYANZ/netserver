
#include <time.h>
class TimeStamp
{
private:
    time_t Stamp_;
public:
    TimeStamp();

    void updateStamp();
    time_t Stamp();
    time_t now();
    ~ TimeStamp();
};