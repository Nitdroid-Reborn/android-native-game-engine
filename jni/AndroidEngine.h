#ifndef ENGINE_H
#define ENGINE_H

#include "IEngine.h"
#include <android/sensor.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include "FileIO/IFileIO.h"
#include "ContentManager/PNGLoader.h"
#include "Input/Input.h"
#include <android_native_app_glue.h>
#include <android/log.h>
#include "MyModel.h"
#include <time.h>
#include "VirtualInput/VirtualInput.h"
#include "VirtualInput/VirtualSingleKey.h"
#include "VirtualInput/VirtualDPad.h"
#include "Clock.h"

#include "AudioSystem.h"



struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};


class AndroidEngine : public IEngine
{
public:
    AndroidEngine(android_app* app);
    ~AndroidEngine();

    void Initialize();
    void Release();
    void OnSaveState();
    void OnInitWindow();
    void OnTerminateWindow();
    void OnGainedFocus();
    void OnLostFocus();
    void OnPause();
    void OnResume();
    void OnFrameStart();
    void OnFrameEnd();


    void ProcessTouchInput(const TouchEvent& event);
    void ProcessKeyInput(const KeyEvent& event);
    void ProcessAccelerometerInput(float x, float y, float z);

    void Render();
    void Update(float dt);

    bool IsRunning() const;
    bool IsQuiting() const;

private:
    android_app * app;
    struct saved_state state;


   // AudioSystem audioSystem;
    IFileIO* fileIOSystem;
    Input* inputSystem;
    VirtualInput* virtualInputSystem;


    /*VirtualSingleKey* centerKey;
    VirtualDPad* dpad;
    PNGLoader pngLoader;*/

    bool isRunning;
    bool isQuitting;


    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;

    int InitDisplay();
    void TerminateDisplay();

    int frameCounter;
    float dt;
    Clock fpsClock;
    MyModel model;
  /*  float posX, posY;
    float deltaX, deltaY;


    float touchX[4];
    float touchY[4];
    bool touch[4];
    float lastPosX;*/



};

#endif // ENGINE_H