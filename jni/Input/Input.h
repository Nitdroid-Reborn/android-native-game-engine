#ifndef INPUT_H
#define INPUT_H

#include "InputKeys.h"
#include "KeysState.h"
#include "TouchState.h"


class Input
{
public:
    Input();
    virtual ~Input() {}

    virtual bool Initialize();
    virtual bool Release();

    virtual void ProcessKeyEvent(const KeyEvent&);
    virtual void ProcessTouchEvent(const TouchEvent&);

    virtual void StartFrame();
    virtual void EndFrame();

    const KeysState* GetKeyState() const;
    const TouchState* GetTouchState() const;

    static Input* get() {return singleton;}

protected:
    static Input* singleton;
private:
    KeysState keyState;
    TouchState touchState;
};

#endif // INPUT_H
