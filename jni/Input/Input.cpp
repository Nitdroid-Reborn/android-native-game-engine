#include "Input.h"
#include <Utils/Utils.h>

Input* Input::singleton = NULL;

Input::Input()
{
    pendingKeyEvents.reserve(10);
    pendingTouchEvents.reserve(10);
}


bool Input::Initialize() {
    ASSERT(!singleton, "Input system already initialized");
    singleton = this;

    ScriptManager* manager = ScriptManager::Get();

    manager->RegisterClass<Input>();
    manager->RegisterStaticClassFunction<Input>("Get", InputGet);
    manager->RegisterClass<KeysState>();
    manager->RegisterClass<TouchState>();


{
    lua_State* s = manager->getState();
    OOLUA::push2lua(s, 0);
    lua_setglobal(s, "ENGINE_POINTER_0");
    OOLUA::push2lua(s, 1);
    lua_setglobal(s, "ENGINE_POINTER_1");
    OOLUA::push2lua(s, 2);
    lua_setglobal(s, "ENGINE_POINTER_2");
    OOLUA::push2lua(s, 3);
    lua_setglobal(s, "ENGINE_POINTER_3");
    OOLUA::push2lua(s, 4);
    lua_setglobal(s, "ENGINE_POINTER_4");
    OOLUA::push2lua(s, 5);
    lua_setglobal(s, "ENGINE_POINTER_5");
    OOLUA::push2lua(s, 6);
    lua_setglobal(s, "ENGINE_POINTER_6");
    OOLUA::push2lua(s, 7);
    lua_setglobal(s, "ENGINE_POINTER_7");
    OOLUA::push2lua(s, 8);
    lua_setglobal(s, "ENGINE_POINTER_8");
    OOLUA::push2lua(s, 9);
    lua_setglobal(s, "ENGINE_POINTER_9");
    OOLUA::push2lua(s, 10);
    lua_setglobal(s, "ENGINE_POINTER_10");

    OOLUA::push2lua(s, ENGINE_KEYCODE_UNKNOWN);
    lua_setglobal(s, "ENGINE_KEYCODE_UNKNOW");
    OOLUA::push2lua(s, ENGINE_KEYCODE_UP);
    lua_setglobal(s, "ENGINE_KEYCODE_UP");
    OOLUA::push2lua(s, ENGINE_KEYCODE_DOWN);
    lua_setglobal(s, "ENGINE_KEYCODE_DOWN");
    OOLUA::push2lua(s, ENGINE_KEYCODE_LEFT);
    lua_setglobal(s, "ENGINE_KEYCODE_LEFT");
    OOLUA::push2lua(s, ENGINE_KEYCODE_RIGHT);
    lua_setglobal(s, "ENGINE_KEYCODE_RIGHT");
    OOLUA::push2lua(s, ENGINE_KEYCODE_CENTER);
    lua_setglobal(s, "ENGINE_KEYCODE_CENTER");
    OOLUA::push2lua(s, ENGINE_KEYCODE_ENTER);
    lua_setglobal(s, "ENGINE_KEYCODE_ENTER");
    OOLUA::push2lua(s, ENGINE_KEYCODE_M);
    lua_setglobal(s, "ENGINE_KEYCODE_M");
    OOLUA::push2lua(s, ENGINE_KEYCODE_N);
    lua_setglobal(s, "ENGINE_KEYCODE_N");


    }


    Logger::Log(1, "Input system initialized");
    return true;
}

bool Input::Release() {
    singleton = NULL;
    Logger::Log(1, "Input system released");
    return true;
}

const KeysState* Input::GetKeyState() const {
    return &keyState;
}

const TouchState* Input::GetTouchState() const {
    return &touchState;
}

void Input::ProcessKeyEvent(const KeyEvent &event) {
    mutex.Lock();
    pendingKeyEvents.push_back(event);
    mutex.Unlock();
}

void Input::ProcessTouchEvent(const TouchEvent &event) {
    mutex.Lock();
    pendingTouchEvents.push_back(event);
    mutex.Unlock();
}


void Input::EndFrame() {
    keyState.EndFrame();
    touchState.EndFrame();
}

void Input::StartFrame() {
    mutex.Lock();
    for(int i=0;i<pendingKeyEvents.size();i++) {
        keyState.NewEvent(pendingKeyEvents[i]);
    }
    pendingKeyEvents.clear();

    for(int i=0;i<pendingTouchEvents.size();i++) {
        touchState.NewEvent(pendingTouchEvents[i]);
    }
    pendingTouchEvents.clear();
    mutex.Unlock();

    keyState.StartFrame();
    touchState.StartFrame();
}

int InputGet(lua_State *l) {
    OOLUA_C_FUNCTION(Input*, Input::get)
}

EXPORT_OOLUA_FUNCTIONS_NON_CONST(Input)
EXPORT_OOLUA_FUNCTIONS_CONST(Input, GetKeyState, GetTouchState)
