#ifndef ENGINE_H
#define ENGINE_H

#include "IEngine.h"
#include <android/sensor.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <FileIO/IFileIO.h>
#include <ContentManager/PNGLoader.h>
#include <Input/Input.h>
#include <android_native_app_glue.h>
#include <android/log.h>
#include "MyModel.h"
#include <time.h>
#include "VirtualInput/VirtualInput.h"
#include "VirtualInput/VirtualSingleKey.h"
#include "VirtualInput/VirtualDPad.h"
#include <Utils/Clock.h>
#include <Core/Mutex.h>
#include <Graphics/AndroidRenderer.h>
#include <Graphics/ITexture.h>
#include <ContentManager/AndroidContentManager.h>
#include <Audio/AndroidAudioSystem.h>
//#include "Box2D/Box2D.h"



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

    void SingleFrame();
    void ProcessTouchInput(const TouchEvent& event);
    void ProcessKeyInput(const KeyEvent& event);
    void ProcessAccelerometerInput(float x, float y, float z);

    void Update(float dt);

    bool IsRunning();
    bool IsQuiting();

    bool buffersSwapped;

    IRenderer* renderer;

private:
    android_app * app;
    struct saved_state state;
    bool closeEngine;


    AndroidAudioSystem* audioSystem;
    IFileIO* fileIOSystem;
    Input* inputSystem;

    VirtualInput* virtualInputSystem;


    /*VirtualSingleKey* centerKey;
    VirtualDPad* dpad;
    PNGLoader pngLoader;*/

    bool isRunning;
    bool isQuitting;

    ITexture* texture;
    ISpriteBatcher* spriteBatcher;
    IContentManager* contentManager;


    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;


    SoundHandle sound1;
    SoundHandle sound2;

    int InitDisplay();
    void TerminateDisplay();

    int frameCounter;
    float dt;
    Clock fpsClock;
    U64 currentTime;
    U64 lastTime;

    bool initDisp;
    bool termDisp;

    Mutex mutex;
    Mutex windowMutex;

    float volume;

};

#endif // ENGINE_H
