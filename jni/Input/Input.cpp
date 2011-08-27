#include "Input.h"
#include <assert.h>
#include "Utils.h"

Input* Input::singleton = NULL;

Input::Input()
{
    pendingKeyEvents.reserve(10);
    pendingTouchEvents.reserve(10);
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
    mutex.Lock();
    pendingKeyEvents.push_back(event);
    mutex.Unlock();
}

void Input::ProcessTouchEvent(const TouchEvent &event) {
    mutex.Lock();
    pendingTouchEvents.push_back(event);
    mutex.Unlock();
}


void Input::EndFrame() {
    keyState.EndFrame();
    touchState.EndFrame();
}

void Input::StartFrame() {
    mutex.Lock();
    for(int i=0;i<pendingKeyEvents.size();i++) {
        keyState.NewEvent(pendingKeyEvents[i]);
    }
    pendingKeyEvents.clear();

    for(int i=0;i<pendingTouchEvents.size();i++) {
        touchState.NewEvent(pendingTouchEvents[i]);
    }
    pendingTouchEvents.clear();
    mutex.Unlock();

    keyState.StartFrame();
    touchState.StartFrame();
}
