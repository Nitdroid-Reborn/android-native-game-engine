#ifndef INPUT_H
#define INPUT_H

#include "InputKeys.h"
#include "KeysState.h"
#include "TouchState.h"


class Input
{
public:
    Input();

    void ProcessKeyEvent(const KeyEvent&);
    void ProcessTouchEvent(const TouchEvent&);

    void StartFrame();
    void EndFrame();

    const KeysState* GetKeyState() const;
    const TouchState* GetTouchState() const;

private:
    KeysState keyState;
    TouchState touchState;
};

#endif // INPUT_H
