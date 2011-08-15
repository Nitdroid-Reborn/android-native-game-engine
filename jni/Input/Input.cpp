#include "Input.h"
#include <assert.h>

Input* Input::singleton = NULL;

Input::Input()
{
}


bool Input::Initialize() {
    assert(!singleton);
    singleton = this;
    return true;
}

bool Input::Release() {
    singleton = NULL;
    return true;
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
