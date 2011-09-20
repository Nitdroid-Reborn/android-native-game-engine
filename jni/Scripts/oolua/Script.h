#ifndef SCRIPT_H
#define SCRIPT_H
extern "C" {
    #include <lua.h>
}

#include <oolua.h>
#include <lua_function.h>
#include <string>

class Script
{
public:
    Script();
    ~Script();

    bool runFile(char *fileName);
    bool runString(const std::string& command);
    OOLUA::Lua_function callFunction;

//private:
    lua_State* threadState;
    char lastErrorString[256];
    void formatError();
};

#endif // SCRIPT_H
