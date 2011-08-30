#include "Log.h"
#include <android/log.h>

void Log(const char *format...) {
    logMutex.Lock();
    va_list argList;
    va_start(argList, format);

    Log(format, argList);

    va_end(argList);
    logMutex.Unlock();
}

void Log(const char *format, va_list argList) {

    const U32 MAX_CHARS=1023;

    static char buffer[MAX_CHARS+1];

    vsnprintf(buffer, MAX_CHARS, format, argList);
    buffer[MAX_CHARS] = '\0';

    __android_log_print(ANDROID_LOG_INFO, "native-activity", buffer);

}

void Log(U32 verbosity, const char *format...) {
    logMutex.Lock();
    if(verbosity>=verbosityLevel) {
        va_list argList;
        va_start(argList, format);

        Log(format, argList);

        va_end(argList);
    }
    logMutex.Unlock();
}
