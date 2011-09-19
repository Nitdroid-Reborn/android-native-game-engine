#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H


extern "C" {
    #include <Scripts/lua/lua.h>
    #include <Scripts/lua/lualib.h>
    #include <Scripts/lua/lauxlib.h>
}
#include <Scripts/oolua/oolua.h>


class ScriptManager
{
public:
    ScriptManager();
    void Initialize();
    void Release();

    inline lua_State* getState() const {return mainState;}

    static ScriptManager* Get() { return singleton;}

    template<typename T>
    void RegisterClass() {
        OOLUA::register_class<T>(mainState);
    }

    template<typename T, typename K, typename V>
    void RegisterStaticClassFunction(K const& k, V const& v) {
        OOLUA::register_class_static<T, K, V>(mainState, k, v);
    }

private:
    static ScriptManager* singleton;
    lua_State* mainState;
};

#endif // SCRIPTMANAGER_H
