#include "KeysState.h"
#include <Utils/Utils.h>
KeysState::KeysState() {
    keyStates = 0x0;
    previousKeyStates = 0x0;
}

void KeysState::DetectKeyUpDownEvents() {
    U64 keysChanges = keyStates ^ previousKeyStates;

    keyDowns = keysChanges & keyStates;
    keyUps = keysChanges & (~keyStates);
}

bool KeysState::IsKeyPressed(EngineKeyCode key) const {
    U64 k = 1LL << key;
    return (keyStates & k);
}

bool KeysState::IsKeyJustPressed(EngineKeyCode key) const {
    U64 k = 1LL << key;
    return (keyDowns & k);
}

bool KeysState::IsKeyJustReleased(EngineKeyCode key) const {
    U64 k = 1LL << key;
    return (keyUps & k);
}


void KeysState::NewEvent(const KeyEvent &event) {
    U64 k = 1LL << event.keyCode;
    if(event.action == ENGINE_KEYACTION_DOWN)
        keyStates |= k;
    else
        keyStates = keyStates & (~k);
}

void KeysState::StartFrame() {
    DetectKeyUpDownEvents();
}

void KeysState::EndFrame() {
    previousKeyStates = keyStates;
}

EXPORT_OOLUA_FUNCTIONS_NON_CONST(KeysState)
EXPORT_OOLUA_FUNCTIONS_CONST(KeysState, IsKeyPressed, IsKeyJustPressed, IsKeyJustReleased)
