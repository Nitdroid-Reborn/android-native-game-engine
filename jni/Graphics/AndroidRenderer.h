#ifdef ANDROID
#ifndef ANDROIDRENDERER_H
#define ANDROIDRENDERER_H
#include "IRenderer.h"
#include <Core/Mutex.h>
#include <EGL/egl.h>
#include "android_native_app_glue.h"
#include <Utils/Clock.h>
#include <Core/ConditionalVariable.h>
#include <vector>
#include "SpriteBatcher.h"
#include <ContentManager/IContentManager.h>
#include "OGLTexture.h"
#include "TextBox.h"
#include "Font.h"

using namespace std;

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
    void DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, F32 angle=0.0f);
    void DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, U8 r, U8 g, U8 b, U8 a, F32 angle=0.0f);
    void DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, TextureRegion& region, TextureHandle& handle, F32 angle=0.0f);
    void DrawString(int x, int y, const char * str);

    TextureHandle fontTexture;

    Font myFont;
private:
    void InitWindow();
    void TerminateWindow();


    android_app* app;

    bool contextValid;
    bool initWindow;
    bool terminateWindow;
    bool closing;
    Mutex mutex;
    Mutex queueMutex;
    bool active;

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;



    TextBox textBox;

    int32_t width;


    U64 currentTime;
    U64 lastTime;
    int32_t height;
    int frameCounter;
    float dt;
    Clock fpsClock;
    ConditionalVariable mainLoopCond;
    SpriteBatcher* batcher;   
};

#endif // ANDROIDRENDERER_H
#endif
