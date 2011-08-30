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
    return (keyStates & key);
}

bool KeysState::IsKeyJustPressed(EngineKeyCode key) const {
    return (keyDowns & key);
}

bool KeysState::IsKeyJustReleased(EngineKeyCode key) const {
    return (keyUps & key);
}


void KeysState::NewEvent(const KeyEvent &event) {
    if(event.action == ENGINE_KEYACTION_DOWN)
        keyStates |= event.keyCode;
    else
        keyStates = keyStates & (~event.keyCode);
}

void KeysState::StartFrame() {
    DetectKeyUpDownEvents();
}

void KeysState::EndFrame() {
    previousKeyStates = keyStates;
}
