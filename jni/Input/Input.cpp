#include "Input.h"

Input::Input()
{
}


const KeysState* Input::GetKeyState() const {
    return &keyState;
}

const TouchState* Input::GetTouchState() const {
    return &touchState;
}

void Input::ProcessKeyEvent(const KeyEvent &event) {
    keyState.NewEvent(event);
}

void Input::ProcessTouchEvent(const TouchEvent &event) {
    touchState.NewEvent(event);
}


void Input::EndFrame() {
    keyState.EndFrame();
    touchState.EndFrame();
}

void Input::StartFrame() {
    keyState.StartFrame();
    touchState.StartFrame();
}
