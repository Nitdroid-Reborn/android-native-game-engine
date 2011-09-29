#include "KeysState.h"
#include <Utils/Utils.h>

KeysState::KeysState() {
    //nothing pressed
    keyStates = 0x0;
    previousKeyStates = 0x0;
}

void KeysState::DetectKeyUpDownEvents() {
    //difference from last frame
    U64 keysChanges = keyStates ^ previousKeyStates;

    //which keys just went down
    keyDowns = keysChanges & keyStates;
    //which keys just went up
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
    //changes key state
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
