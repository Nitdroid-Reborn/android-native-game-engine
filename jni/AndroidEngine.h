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
#include "Mutex.h"
#include "AndroidRenderer.h"
#include "Graphics/ITexture.h"
#include "Graphics/ISpriteBatcher.h"

#include "AudioSystem.h"


#include "Box2D/Box2D.h"



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

    void Render();

    void Run();

    void ProcessTouchInput(const TouchEvent& event);
    void ProcessKeyInput(const KeyEvent& event);
    void ProcessAccelerometerInput(float x, float y, float z);

    void Update(float dt);

    bool IsRunning();
    bool IsQuiting();

private:
    android_app * app;
    struct saved_state state;
    bool closeEngine;


   // AudioSystem audioSystem;
    IFileIO* fileIOSystem;
    Input* inputSystem;
    IRenderer* renderer;
    VirtualInput* virtualInputSystem;


    /*VirtualSingleKey* centerKey;
    VirtualDPad* dpad;
    PNGLoader pngLoader;*/

    bool isRunning;
    bool isQuitting;

    ITexture* texture;
    ISpriteBatcher* spriteBatcher;


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

    bool initDisp;
    bool termDisp;

    Mutex mutex;
    Mutex windowMutex;

};

#endif // ENGINE_H
