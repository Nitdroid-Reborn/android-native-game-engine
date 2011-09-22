#ifndef KEYSSTATE_H
#define KEYSSTATE_H
#include "InputKeys.h"
#include <Scripts/ScriptManager.h>

class KeysState
{
public:
    KeysState();

    bool IsKeyPressed(EngineKeyCode key) const;
    bool IsKeyJustPressed(EngineKeyCode key) const;
    bool IsKeyJustReleased(EngineKeyCode key) const;

    void StartFrame();
    void EndFrame();

    void NewEvent(const KeyEvent& event);

private:
    void DetectKeyUpDownEvents();
    U64 keyStates;
    U64 previousKeyStates;
    U64 keyDowns;
    U64 keyUps;
};

/*OOLUA_PROXY_CLASS(KeysState)
    OOLUA_NO_TYPEDEFS
    OOLUA_ONLY_DEFAULT_CONSTRUCTOR
    OOLUA_MEM_FUNC_1_CONST(bool, IsKeyPressed, U8)
    OOLUA_MEM_FUNC_1_CONST(bool, IsKeyJustPressed, U8)
    OOLUA_MEM_FUNC_1_CONST(bool, IsKeyJustReleased, U8)
OOLUA_CLASS_END*/

#endif // KEYSSTATE_H
