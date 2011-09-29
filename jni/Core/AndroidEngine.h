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
#include <Scripts/Script.h>

//#include "Box2D/Box2D.h"


//! Represents game save state.
/*!
  This state is saved automatically between game runs.
  */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

//! Implementation of IEngine interface with android ndk
class AndroidEngine : public IEngine
{
public:
    //! Constructor
    /*!
      \param app android_app pointer with main Android native application
      */
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
    void SingleFrame();
    void ProcessTouchInput(const TouchEvent& event);
    void ProcessKeyInput(const KeyEvent& event);
    void ProcessAccelerometerInput(float x, float y, float z);

    void Update(float dt);

    bool IsRunning();
    bool IsQuiting();

    /** Render component of engine*/
    IRenderer* renderer;

private:
    /// Pointer to app
    android_app * app;
    struct saved_state state;


    /// Audio component of engine
    AndroidAudioSystem* audioSystem;
    /// File IO component of engine
    IFileIO* fileIOSystem;

    /// Input component of engine
    Input* inputSystem;

    /// Content manager component of engine
    IContentManager* contentManager;

    /// Script manager component of engine
    ScriptManager* scriptManager;

    VirtualInput* virtualInputSystem;


    float angle;

    VirtualSingleKey* centerKey;
    VirtualDPad* dpad;

    /// Flags indicating if engine is running or is quitting
    bool isRunning;
    bool isQuitting;

    //some test values
    Script* script;
    //SoundHandle* sound1;
   // SoundHandle* sound2;

    TextureHandle texture;

    int frameCounter;
    float dt;
    Clock fpsClock;
    U64 currentTime;
    U64 lastTime;

    float volume;
};

#endif // ENGINE_H
