#ifndef IRENDERER_H
#define IRENDERER_H

#include "Thread.h"
#include "Utils.h"

class IRenderer : public Thread
{
public:
    IRenderer() : Thread(){}
    virtual ~IRenderer(){}

    virtual void Initialize()=0;
    virtual void Release()=0;
    virtual void Run()=0;

    virtual void OnInitWindow()=0;
    virtual void OnTerminateWindow()=0;
    virtual void OnGainedFocus()=0;
    virtual void OnLostFocus()=0;
    virtual void Wait()=0;
    virtual void DrawSprite(F32 x, F32 y, F32 width, F32 height, F32 angle=0.0f)=0;
};

#endif // IRENDERER_H
