#ifndef KEYSSTATE_H
#define KEYSSTATE_H
#include "InputKeys.h"
#include <Scripts/ScriptManager.h>

//! Tracks and updates state of keys
class KeysState
{
public:
    KeysState();
    //! Checks if key is currently pressed
    /*!
      \param key key code of key in interest
      */
    bool IsKeyPressed(EngineKeyCode key) const;

    //! Checks if key was just pressed
    /*!
      It's only true in frame when state of key changed from up to down
      \param key key code of key in interest
      */
    bool IsKeyJustPressed(EngineKeyCode key) const;

    //! Checks if key was just released
    /*!
      It's only true in frame when state of key changed from down to up
      \param key key code of key in interest
      */
    bool IsKeyJustReleased(EngineKeyCode key) const;

    //! Calculates changes of key states from last frame
    void StartFrame();

    //! Saves state of keys in this frame
    void EndFrame();

    //! Process key event and updates keys state
    void NewEvent(const KeyEvent& event);

private:
    void DetectKeyUpDownEvents();
    //key state is coded in 64-bit int
    //when bit is set key is pressed
    //bit of each key can be calculated as 1<<keyCode
    U64 keyStates;
    U64 previousKeyStates;
    U64 keyDowns;
    U64 keyUps;
};
#endif // KEYSSTATE_H
