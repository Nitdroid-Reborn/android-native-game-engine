#ifndef UTILS_H
#define UTILS_H

#include "EngineTypes.h"
#include "Log.h"
#include "Assert.h"
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))

U64 GetCurrentTimeInMsec();
timespec GetCurrenTime();
float GetDiffInMsec(const timespec& start, const timespec& end);

#endif // UTILS_H
