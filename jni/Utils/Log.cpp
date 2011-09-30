#include "Log.h"
#include "Utils.h"


void Logger::Log(const char *format...) {
    va_list argList;
    va_start(argList, format);

    Log(LOG_INFO, format, argList);

    va_end(argList);
}

void Logger::Log(LogType logType, const char *format...) {
    va_list argList;
    va_start(argList, format);

    Log(logType, format, argList);

    va_end(argList);
}

void Logger::LuaLog(const char *message) {
    if(message)
        LOGI(message);
}

void Logger::Log(LogType logType, const char *format, va_list argList) {

    const U32 MAX_CHARS=1023;

    static char buffer[MAX_CHARS+1];

    vsnprintf(buffer, MAX_CHARS, format, argList);
    buffer[MAX_CHARS] = '\0';

#ifdef ANDROID
    __android_log_print(logType, "native-activity", buffer);
#else
    LOGI(buffer);
#endif

}

void Logger::Log(U32 verbosity, const char *format...) {
    if(verbosity>=verbosityLevel) {
        va_list argList;
        va_start(argList, format);

        Log(LOG_INFO, format, argList);

        va_end(argList);
    }
}

void Logger::Log(LogType logType, U32 verbosity, const char *format...) {
    if(verbosity>=verbosityLevel) {
        va_list argList;
        va_start(argList, format);

        Log(logType, format, argList);

        va_end(argList);
    }
}
