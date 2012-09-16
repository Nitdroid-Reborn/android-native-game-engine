#ifndef QTRENDERER_H
#define QTRENDERER_H
#include "IRenderer.h"


#include "Font.h"
#include "SpriteBatcher.h"
#include "TextBox.h"
#include "Utils/Profiler.h"
#include <Utils/Clock.h>
#include <Math/MathLib.h>
#include "Camera.h"

class QGLWidget;
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
    Camera* camera;
    Camera* mainThreadCamera;

    float angle;
    std::vector<GeometryInstance> geometry;
    std::vector<GeometryInstance> oldGeometry;

    std::vector<GeometryInstance> alphaGeometry;
    std::vector<GeometryInstance> oldAlphaGeometry;
};

#endif // QTRENDERER_H
