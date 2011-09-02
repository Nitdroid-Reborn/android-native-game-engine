#ifndef RELATIVECLOCK_H
#define RELATIVECLOCK_H
#include <Utils/Utils.h>

class RelativeClock
{
public:
    RelativeClock();
    float GetTime();
    void Init();

private:
    timespec start;
};

#endif // RELATIVECLOCK_H
