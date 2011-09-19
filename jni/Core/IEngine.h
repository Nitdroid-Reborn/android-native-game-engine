#ifndef IENGINE_H
#define IENGINE_H
#include "Thread.h"
#include "Input/InputKeys.h"


/** Interface of engine*/
class IEngine {
public:
    /** Defaul constructor*/
    IEngine() {}
    virtual ~IEngine(){}

    /** Initialization of engine, it should initalize all subsustems */
    virtual void Initialize()=0;
    /** Release of engine, it should release all subsuystems*/
    virtual void Release()=0;

    /** Callbacks for system events*/

    /** Called when app is closing for save state of game*/
    virtual void OnSaveState()=0;

    /** Called after creation of main window, it's place where rendering
        context should be initialized*/
    virtual void OnInitWindow()=0;

    /** Called after termination of main window, all rendering resources
        should be released here */
    virtual void OnTerminateWindow()=0;

    /** Called when app has focus */
    virtual void OnGainedFocus()=0;

    /** Called when app lost focus (we should e.g. stop pooling accelerometer data)*/
    virtual void OnLostFocus()=0;

    /** Called when game lost focus and is moved background*/
    virtual void OnPause()=0;

    /** Called when game is on foreground and has focus*/
    virtual void OnResume()=0;

    /** Called every time when new frame starts*/
    virtual void OnFrameStart()=0;

    /** Called every time when frame ends*/
    virtual void OnFrameEnd()=0;

    /** Updates all components*/
    virtual void SingleFrame()=0;

    /** Process new touch input*/

    virtual void ProcessTouchInput(const TouchEvent&)=0;

    /** Process new key input*/
    virtual void ProcessKeyInput(const KeyEvent&)=0;

    /** Process accelerometer data*/
    virtual void ProcessAccelerometerInput(float x, float y, float z)=0;

    /** Update game logic*/
    virtual void Update(float dt)=0;
    //virtual void Render()=0;

    /** Indicating if engine is running*/
    virtual bool IsRunning()=0;

    /** Indicating if engine is quitting*/
    virtual bool IsQuiting()=0;
};

#endif // IENGINE_H
