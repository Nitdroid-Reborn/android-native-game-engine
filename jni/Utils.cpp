#include "Utils.h"
#include <time.h>

double getCurrentTime() {
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (double)t.tv_sec + (double)t.tv_nsec/1e9;
}
