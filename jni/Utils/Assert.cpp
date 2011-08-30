#include "Assert.h"
#include <android/log.h>
#include <stdlib.h>

void Assert(const char* fileName, const int line, const char* message) {
    __android_log_print(ANDROID_LOG_ERROR, "native-activity", "ASSERT FAILED: %s (%d) : %s", fileName, line, message);
    abort();
}
