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
    void DrawSprite(F32 x, F32 y, F32 width, F32 height, F32 angle=0.0f);
    void DrawSprite(F32 x, F32 y, F32 width, F32 height, TextureRegion& region, TextureHandle& handle, F32 angle=0.0f);
    void DrawString(int x, int y, const char * str);

    TextureHandle fontTexture;
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


    struct Sprite {
        F32 x;
        F32 y;
        F32 width;
        F32 height;
        F32 angle;
        TextureRegion texRegion;
        ITexture* texture;

        bool operator<(const Sprite& rhs) const{
            return texture>rhs.texture;
        }
    };

    vector<Sprite> sprites;
    vector<Sprite> oldSprites;    
};

#endif // ANDROIDRENDERER_H
