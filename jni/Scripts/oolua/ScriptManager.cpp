#include "ScriptManager.h"
#include <QDebug>
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
    }
ScriptManager* ScriptManager::singleton;

ScriptManager::ScriptManager()
{

}

void ScriptManager::Initialize() {

    singleton = this;

    mainState = luaL_newstate();

    luaL_openlibs(mainState);

    int top = lua_gettop(mainState);
    if(top != 0)lua_pop(mainState,top);
}

void ScriptManager::Release() {
    lua_close(mainState);
}
