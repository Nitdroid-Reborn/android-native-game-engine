#include "Assert.h"
#ifdef ANDROID
#include <android/log.h>
#else
#include <QDebug>
#endif
#include <stdlib.h>

void Assert(const char* fileName, const int line, const char* message) {
#ifdef ANDROID
    __android_log_print(ANDROID_LOG_ERROR, "native-activity", "ASSERT FAILED: %s", fileName);
    __android_log_print(ANDROID_LOG_ERROR, "native-activity", "ASSERT FAILED: (%d)", line);
    __android_log_print(ANDROID_LOG_ERROR, "native-activity", "ASSERT FAILED: %s", message);
#else
    qDebug("ASSERT FAILED: %s (%d) : %s", fileName, line, message);
#endif
    abort();
}
