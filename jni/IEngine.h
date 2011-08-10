#ifndef IENGINE_H
#define IENGINE_H

#include "Input/InputKeys.h"

class IEngine {
public:
    IEngine(){}
    virtual ~IEngine(){}

    virtual void Initialize()=0;
    virtual void Release()=0;

    virtual void OnSaveState()=0;
    virtual void OnInitWindow()=0;
    virtual void OnTerminateWindow()=0;
    virtual void OnGainedFocus()=0;
    virtual void OnLostFocus()=0;
    virtual void OnPause()=0;
    virtual void OnResume()=0;
    virtual void OnFrameStart()=0;
    virtual void OnFrameEnd()=0;

    virtual void ProcessTouchInput(const TouchEvent&)=0;
    virtual void ProcessKeyInput(const KeyEvent&)=0;
    virtual void ProcessAccelerometerInput(float x, float y, float z)=0;
    virtual void Update(float dt)=0;
    virtual void Render()=0;

    virtual bool IsRunning() const=0;
    virtual bool IsQuiting() const=0;
};

#endif // IENGINE_H
