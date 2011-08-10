#ifndef KEYSSTATE_H
#define KEYSSTATE_H
#include "InputKeys.h"

class KeysState
{
public:
    KeysState();

    bool IsKeyPressed(EngineKeyCode key) const;
    bool IsKeyJustPressed(EngineKeyCode key) const;

    void DetectKeyUpDownEvents();
    void EndFrame();
    void NewEvent(const KeyEvent& event);

private:
    U64 keyStates;
    U64 previousKeyStates;
    U64 keyDowns;
    U64 keyUps;
};

#endif // KEYSSTATE_H
