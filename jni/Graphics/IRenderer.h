#ifndef IRENDERER_H
#define IRENDERER_H

#include <Core/Thread.h>
#include <Utils/Utils.h>
#include "TextureRegion.h"
#include <ContentManager/ITextureManager.h>
#include <Scripts/ScriptManager.h>

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
    virtual void DrawSprite(F32 x, F32 y, F32 width, F32 height, F32 angle=0.0f)=0;
    virtual void DrawSprite(F32 x, F32 y, F32 width, F32 height, TextureRegion& region, TextureHandle& handle, F32 angle=0.0f)=0;
    virtual void DrawString(int x, int y, char * str)=0;

    static IRenderer* get(){return singleton;}

protected:
    static IRenderer* singleton;
};

int IRendererGet(lua_State* l);

/*OOLUA_PROXY_CLASS(IRenderer)
    OOLUA_MEM_FUNC_3(void, DrawString, int, int, char*)
    OOLUA_MEM_FUNC_5_RENAME(DrawSprite, void, DrawSprite, F32, F32, F32, F32, F32)
    OOLUA_MEM_FUNC_7_RENAME(DrawTexturedSprite, void, DrawSprite, F32, F32, F32, F32, TextureRegion&, TextureHandle&, F32)
    OOLUA_TYPEDEFS Abstract OOLUA_END_TYPES
OOLUA_CLASS_END*/

#endif // IRENDERER_H
