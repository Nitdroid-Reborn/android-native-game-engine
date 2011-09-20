#ifndef SCRIPT_H
#define SCRIPT_H
extern "C" {
    #include <Scripts/lua/lua.h>
}

//#include <Scripts/oolua/oolua.h>
//#include <Scripts/oolua/lua_function.h>
#include <string>
#include <Utils/Log.h>

void PrintLastError(lua_State* s);

struct Lua_function_checking_errors {
    //OOLUA::Lua_function function;

   /* template<typename FUNC>
    bool operator()(FUNC const&  func_name)
    {
        if(!function(func_name))
            PrintLastError(state);
    }
    template<typename FUNC,typename P1>
    bool operator()(FUNC const&  func_name,P1 p1)
    {
        if(!function(func_name, p1))
            PrintLastError(state);
    }
    template<typename FUNC,typename P1,typename P2>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2)
    {
        if(!function(func_name, p1, p2))
            PrintLastError(state);
    }
    template<typename FUNC,typename P1,typename P2,typename P3>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3)
    {
        if(!function(func_name, p1, p2, p3))
            PrintLastError(state);
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4)
    {
        if(!function(func_name, p1, p2, p3, p4))
            PrintLastError(state);
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
    {
        if(!function(func_name, p1, p2, p3, p4, p5))
            PrintLastError(state);
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)
    {
        if(!function(func_name, p1, p2, p3, p4, p5, p6))
            PrintLastError(state);
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)
    {
        if(!function(func_name, p1, p2, p3, p4, p5, p6, p7))
            PrintLastError(state);
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8)
    {
        if(!function(func_name, p1, p2, p3, p4, p5, p6, p7, p8))
            PrintLastError(state);
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8,typename P9>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9)
    {
        if(!function(func_name, p1, p2, p3, p4, p5, p6, p7, p8, p9))
            PrintLastError(state);
    }
    template<typename FUNC,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8,typename P9,typename P10>
    bool operator()(FUNC const&  func_name,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8,P9 p9,P10 p10)
    {
        if(!function(func_name, p1, p2, p3, p4, p5, p6, p7, p8, p9))
            PrintLastError(state);
    }
*/
    lua_State* state;
};

class Script
{
public:
    Script();
    ~Script();

    bool runFile(char *fileName);
    bool runString(const std::string& command);
    Lua_function_checking_errors callFunction;

//private:
    lua_State* threadState;
    char lastErrorString[256];
    void formatError();
};

#endif // SCRIPT_H
