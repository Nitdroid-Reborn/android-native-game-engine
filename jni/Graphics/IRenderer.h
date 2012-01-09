#ifndef IRENDERER_H
#define IRENDERER_H

#include <Core/Thread.h>
#include <Utils/Utils.h>
#include "TextureRegion.h"
#include <ContentManager/ITextureManager.h>
#include <Scripts/ScriptManager.h>
#include <ContentManager/ShaderProgramManager.h>
#include <ContentManager/ModelGeometryManager.h>
#include <Graphics/Camera.h>

#define NORMAL_LAYER 10
#define VIRTUAL_INPUT_LAYER 2
#define VIRTUAL_INPUT_LAYER_2 1.9
#define PROFILER_LAYER_BG 1
#define PROFILER_LAYER 0
#define FBO_LAYER -1

class IRenderer : public Thread
{
public:
    IRenderer() : Thread(){}
    virtual ~IRenderer(){}

    virtual void Initialize()=0;
    virtual void Release()=0;
    virtual void Run()=0;

    virtual void OnInitWindow()=0;
    virtual void OnTerminateWindow()=0;
    virtual void OnGainedFocus()=0;
    virtual void OnLostFocus()=0;
    virtual void Wait()=0;
    virtual void DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, F32 angle=0.0f)=0;
    virtual void DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, U8 r, U8 g, U8 b, U8 a, F32 angle=0.0f)=0;
    virtual void DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, TextureRegion& region, TextureHandle& handle, F32 angle=0.0f)=0;
    virtual void DrawSprite(F32 x, F32 y, F32 layer, F32 width, F32 height, TextureRegion& region, TextureHandle& handle, U8 r, U8 g, U8 b, U8 a, F32 angle=0.0f)=0;
    virtual void DrawString(int x, int y, const char * str)=0;
    virtual void DrawString(int x, int y, const char * str, U8 r, U8 g, U8 b)=0;
    virtual void DrawGeometry(ModelGeometryHandle geometry, const Matrix4x4& worldMatrix,
                              ShaderProgramHandle shaderProgram, const ShaderParametersList* shaderParameters,
                              bool transparent=false)=0;
    virtual void DrawGeometry(ModelGeometryHandle geometry, const Matrix4x4& worldMatrix,
                              ShaderProgramHandle shaderProgram, bool transparent=false)=0;
    virtual Camera* GetCamera()=0;

    static IRenderer* get(){return singleton;}

protected:
    static IRenderer* singleton;
};

#endif // IRENDERER_H
