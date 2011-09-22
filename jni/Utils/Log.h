#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "EngineTypes.h"
#include <Core/Mutex.h>
#include <Scripts/ScriptManager.h>

static U32 verbosityLevel = 1;

enum LogType {
    LOG_INFO = 4,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
};

class Logger {
public:
    static void Log(LogType logType, const char* format, __va_list argList);
    static void Log(const char* format, ...);
    static void Log(LogType logType, const char* format, ...);

    static void Log(U32 verbosity, const char* format, ...);
    static void Log(LogType logType, U32 verbosity, const char* format, ...);

    static void LuaLog(const char* msg=0);
};



#endif // LOG_H
