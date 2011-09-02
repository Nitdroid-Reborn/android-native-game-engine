#include "RelativeClock.h"

RelativeClock::RelativeClock()
{

}

void RelativeClock::Init() {
    start = GetCurrenTime();
}

float RelativeClock::GetTime() {
    timespec t = GetCurrenTime();
    return GetDiffInMsec(start, t);
}
