#ifndef UTILS_H
#define UTILS_H

#include <android/log.h>
#include <assert.h>
#include "EngineTypes.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))

U64 getCurrentTimeInMsec();
#endif // UTILS_H
