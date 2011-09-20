#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H


extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}


class ScriptManager
{
public:
    ScriptManager();
    void Initialize();
    void Release();

    inline lua_State* getState() const {return mainState;}

    static ScriptManager* Get() { return singleton;}

private:
    static ScriptManager* singleton;
    lua_State* mainState;
};

#endif // SCRIPTMANAGER_H
