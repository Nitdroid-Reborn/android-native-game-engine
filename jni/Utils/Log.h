#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "EngineTypes.h"
#include <Core/Mutex.h>
//#include <Scripts/oolua/oolua.h>
#include <Scripts/lua/lua.h>

static U32 verbosityLevel = 1;

class Logger {
public:
    static void Log(const char* format, __va_list argList);
    static void Log(const char* format, ...);
    static void Log(U32 verbosity, const char* format, ...);
    static void LuaLog(const char* msg);
};

int Log(lua_State* l);



#endif // LOG_H
