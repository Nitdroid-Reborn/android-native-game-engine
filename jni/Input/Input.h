#ifndef INPUT_H
#define INPUT_H

#include "InputKeys.h"
#include "KeysState.h"


class Input
{
public:

    Input();

    void ProcessKeyEvent(const KeyEvent&);
    void ProcessTouchEvent(const TouchEvent&);

    void StartFrame();
    void EndFrame();

    const KeysState* GetKeyState() const;

private:
    KeysState keyState;
};

#endif // INPUT_H
