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
#include "Camera.h"
#include "ShaderParameter.h"

using namespace std;

class ModelGeometry;
class VBO;
class Shader;
class ShaderProgram;


struct GeometryInstance {
    Matrix4x4 worldMatrix;
    ModelGeometry* geometry;
    ShaderProgram* shaderProgram;
    ShaderParametersList shaderParameters;
};


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
    void DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, TextureRegion& region, TextureHandle& handle, U8 r, U8 g, U8 b, U8 a, F32 angle=0.0f);
    void DrawString(int x, int y, const char * str);
    void DrawString(int x, int y, const char * str, U8 r, U8 g, U8 b);
    void DrawGeometry(ModelGeometryHandle geometry, const Matrix4x4& worldMatrix,
                      ShaderProgramHandle shaderProgram, const ShaderParametersList* shaderParameters,
                      bool transparent=false);
    void DrawGeometry(ModelGeometryHandle geometry, const Matrix4x4& worldMatrix,
                      ShaderProgramHandle shaderProgram, bool transparent=false);

    Camera* GetCamera() {return mainThreadCamera;}

    Font2 myFont;
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


    Camera* camera;
    Camera* mainThreadCamera;

    TextBox textBox;

    int32_t width;

    float lastFps;


    U64 currentTime;
    U64 lastTime;
    int32_t height;
    int frameCounter;
    float dt;
    Clock fpsClock;
    ConditionalVariable mainLoopCond;
    SpriteBatcher* batcher;   


    float angle;
    std::vector<GeometryInstance> geometry;
    std::vector<GeometryInstance> oldGeometry;

    std::vector<GeometryInstance> alphaGeometry;
    std::vector<GeometryInstance> oldAlphaGeometry;
};

#endif // ANDROIDRENDERER_H
#endif
