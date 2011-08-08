#include "Utils.h"
#include <time.h>

U64 getCurrentTimeInMsec() {
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (U64)((double)t.tv_sec*1000 + (double)t.tv_nsec/1e6);
}
