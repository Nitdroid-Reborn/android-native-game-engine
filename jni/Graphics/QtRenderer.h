#ifndef QTRENDERER_H
#define QTRENDERER_H
#include "IRenderer.h"
#include <QGLWidget>
#include "Font.h"
#include "SpriteBatcher.h"
#include "TextBox.h"
#include "Utils/Profiler.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <Utils/Clock.h>

class QtRenderer : public IRenderer
{
public:
    QtRenderer(QGLWidget* app);
    ~QtRenderer(){}

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

    Font myFont;

private:
    QGLWidget* app;
    U64 currentTime;
    U64 lastTime;
    int32_t height;
    int frameCounter;
    float dt;
    Clock fpsClock;
    SpriteBatcher* batcher;
    TextBox textBox;
    bool contextValid;
    bool initWindow;
    bool terminateWindow;
    bool closing;
    bool active;
};

#endif // QTRENDERER_H
