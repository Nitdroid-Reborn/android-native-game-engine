#ifndef ANDROIDRENDERER_H
#define ANDROIDRENDERER_H
#include "IRenderer.h"
#include "Mutex.h"
#include <EGL/egl.h>
#include "android_native_app_glue.h"
#include "Clock.h"
#include "ConditionalVariable.h"

class AndroidRenderer : public IRenderer
{
public:
    AndroidRenderer(android_app* app);
    ~AndroidRenderer(){}

    void Run();

    void Initialize();
    void Release();
    void OnInitWindow();
    void OnTerminateWindow();
    void OnGainedFocus();
    void OnLostFocus();


    void Wait();
private:
    void InitWindow();
    void TerminateWindow();


    android_app* app;

    bool contextValid;
    bool initWindow;
    bool terminateWindow;
    bool closing;
    Mutex mutex;
    bool active;

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;


    U64 currentTime;
    U64 lastTime;
    int32_t height;
    int frameCounter;
    float dt;
    Clock fpsClock;
    ConditionalVariable pauseConditionalVariable;
};

#endif // ANDROIDRENDERER_H
