#include "Utils.h"
#include <time.h>

U64 GetCurrentTimeInMsec() {
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (U64)((double)t.tv_sec*1000 + (double)t.tv_nsec/1e6);
}

timespec GetCurrenTime() {
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t;
}

float GetDiffInMsec(const timespec &start, const timespec &end) {
    timespec res;
    res.tv_sec  = end.tv_sec  - start.tv_sec;
    res.tv_nsec = end.tv_nsec - start.tv_nsec;
    if (res.tv_nsec < 0) {
        res.tv_nsec += 1e9;
        res.tv_sec  -= 1;
    }

    float msec = res.tv_sec*1000;
    msec += (float)res.tv_nsec/1e6;

    return msec;
}
