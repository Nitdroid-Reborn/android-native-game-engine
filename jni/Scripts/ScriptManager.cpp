#include "ScriptManager.h"
#include <Utils/Utils.h>
#include <Utils/Log.h>


ScriptManager* ScriptManager::singleton;

ScriptManager::ScriptManager()
{

}

void ScriptManager::Initialize() {
    ASSERT(!singleton, "Script manager already initialized");
    singleton = this;

    mainState = lua_open();

    luabind::open(mainState);

    if(mainState) {
        luaL_openlibs(mainState);
    }

    Logger::Log(1, "Script manager initialized");
}

void ScriptManager::Release() {
    lua_close(mainState);
    singleton = NULL;
    Logger::Log(1, "Script manager released");
}
