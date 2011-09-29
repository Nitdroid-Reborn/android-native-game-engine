#ifndef IENGINE_H
#define IENGINE_H
#include "Thread.h"
#include "Input/InputKeys.h"


//! Interface of engine
class IEngine {
public:
    //! Defaul constructor
    IEngine() {}
    virtual ~IEngine(){}

    //! Initialization of engine
    /*! It should initalize all subsustems*/
    virtual void Initialize()=0;

    //! Release of engine
    /*! It should release all subsuystems*/
    virtual void Release()=0;


    //! Called when app is closing for save state of game*/
    virtual void OnSaveState()=0;

    //! Called after creation of main window.
    /*! It's place where rendering context should be initialized*/
    virtual void OnInitWindow()=0;

    //! Called after termination of main window
    /*! All rendering resources should be released here, because rendering context can be destroyed by Android */
    virtual void OnTerminateWindow()=0;

    //! Called when app has focus
    /*! We should e.g. start pooling accelerometer data*/
    virtual void OnGainedFocus()=0;

    //! Called when app lost focus
    /*! We should e.g. stop pooling accelerometer data*/
    virtual void OnLostFocus()=0;

    //! Called when game lost focus and is moved background
    virtual void OnPause()=0;

    //! Called when game is on foreground and has focus
    virtual void OnResume()=0;

    //! Called every time when new frame starts
    virtual void OnFrameStart()=0;

    //! Called every time when frame ends
    virtual void OnFrameEnd()=0;

    //! Updates all components
    virtual void SingleFrame()=0;

    //! Process new touch input
    /*!
      \param event new TouchEvent received from main Android activity
      */
    virtual void ProcessTouchInput(const TouchEvent& event)=0;

    //! Process new key input
    /*!
      \param event new KeyEvent received from main Android activity
      */
    virtual void ProcessKeyInput(const KeyEvent&)=0;

    //! Process accelerometer data
    /*!
      \param x accelerometer x-axis value received from main Android activity
      \param y accelerometer x-axis value received from main Android activity
      \param z accelerometer x-axis value received from main Android activity
      */
    virtual void ProcessAccelerometerInput(float x, float y, float z)=0;

    //! Update game logic
    /*!
      \param dt time delta since last update
      */
    virtual void Update(float dt)=0;

    //! Indicating if engine is running
    virtual bool IsRunning()=0;

    //! Indicating if engine is quitting
    virtual bool IsQuiting()=0;
};

#endif // IENGINE_H
