#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "EngineTypes.h"
#include <Core/Mutex.h>


static U32 verbosityLevel = 1;
static Mutex logMutex;

void Log(const char* format, __va_list argList);
void Log(const char* format, ...);
void Log(U32 verbosity, const char* format, ...);
#endif // LOG_H
