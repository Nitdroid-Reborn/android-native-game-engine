#include "Script.h"
#include "ScriptManager.h"
#include <oolua_storage.h>
#include <oolua_check_result.h>
#include <oolua_error.h>
#include <QDebug>

static void stackDump (lua_State *L) {
      int i;
      int top = lua_gettop(L);
      for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {

          case LUA_TSTRING:  /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;

          case LUA_TBOOLEAN:  /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;

          case LUA_TNUMBER:  /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;

          default:  /* other values */
            printf("%s", lua_typename(L, t));
            break;

        }
        printf("  ");  /* put a separator */
      }
      printf("\n");  /* end the listing */
    }

Script::Script() {
    strcpy(lastErrorString, "No error.\n");

    lua_State* masterState = ScriptManager::Get()->getState();

    // create a thread/state for this object
    threadState = lua_newthread(masterState);

    lua_pushlightuserdata(masterState, threadState);
    lua_pushlightuserdata(masterState, this );
    lua_settable(masterState, LUA_GLOBALSINDEX );

    lua_pop(masterState, 1);

    callFunction.bind_script(threadState);
    OOLUA::setup_user_lua_state(threadState);


    lua_pushthread(threadState);
    //thread


    lua_newtable( threadState );
    //thread NewTable


    lua_pushvalue( threadState, -1 );
    //thread NewTable NewTable


    lua_setmetatable( threadState, -2 );
    //thread NewTable

    lua_pushvalue( threadState, LUA_GLOBALSINDEX );
    //thread NewTable GloablTable

    lua_setfield( threadState, -2, "__index" ); //NewTable[__index] = GlobalTable
    //thread NewTable

    lua_setfenv( threadState, -2 ); //thread environment = NewTable
    //thread

    lua_pop(threadState, 1);
    //remove thread from stack
}

Script::~Script() {
    lua_State* masterState = ScriptManager::Get()->getState();
    lua_pushlightuserdata(masterState, threadState);
    lua_pushlightuserdata(masterState, NULL );
    lua_settable(masterState, LUA_GLOBALSINDEX );
}

bool Script::runFile(char *fileName) {
    int result = luaL_loadfile(threadState, fileName);
    bool status = OOLUA::INTERNAL::load_buffer_check_result(threadState, result);
    if(!status)
        return false;

    result = lua_pcall(threadState, 0, LUA_MULTRET, 0);
    return OOLUA::INTERNAL::protected_call_check_result(threadState, result);
}


bool Script::runString(const std::string& command) {
    int res = luaL_loadbuffer(threadState, command.c_str(), command.size(), "userChunk");
    if(!OOLUA::INTERNAL::load_buffer_check_result(threadState,res))
        return false;

    int result = lua_pcall(threadState,0,LUA_MULTRET,0);
    return OOLUA::INTERNAL::protected_call_check_result(threadState,result);
}
