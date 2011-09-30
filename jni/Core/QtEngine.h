#ifndef QTENGINE_H
#define QTENGINE_H
#include "IEngine.h"
#include <QGLWidget>
#include <Utils/Clock.h>
#include <Scripts/ScriptManager.h>
#include <FileIO/QtFileIO.h>
#include <ContentManager/QtContentManager.h>
#include <Graphics/QtRenderer.h>
#include <Audio/QtAudioSystem.h>
#include <Input/Input.h>
#include <Scripts/Script.h>
#include <VirtualInput/VirtualInput.h>
#include <VirtualInput/VirtualDPad.h>
#include <VirtualInput/VirtualSingleKey.h>


class QtEngine : public IEngine
{
public:
    QtEngine(QGLWidget* app);
    ~QtEngine();

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
    QGLWidget* app;
    bool isRunning;
    bool isQuitting;

    int frameCounter;
    float dt;
    Clock fpsClock;
    U64 currentTime;
    U64 lastTime;

    float volume;

    /// Audio component of engine
    IAudioSystem* audioSystem;
    /// File IO component of engine
    IFileIO* fileIOSystem;

    /// Input component of engine
    Input* inputSystem;

    /// Content manager component of engine
    IContentManager* contentManager;

    /// Script manager component of engine
    ScriptManager* scriptManager;
    Script* script;

    TextureHandle texture;

    VirtualInput* virtualInputSystem;
    VirtualSingleKey* centerKey;
    VirtualDPad* dpad;
};

#endif // QTENGINE_H
