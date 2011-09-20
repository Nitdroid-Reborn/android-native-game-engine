#ifndef ITEXTUREMANAGER_H
#define ITEXTUREMANAGER_H

#include "Handle.h"
#include "HandleManager.h"
#include <Graphics/ITextureFactory.h>

struct tagTexture{};

typedef Handle<ITexture, ITextureFactory, tagTexture> TextureHandle;
typedef HandleManager<ITexture, ITextureFactory, tagTexture> TextureHandleManager;

class ITextureManager {
public:
    ITextureManager(){}
    virtual ~ITextureManager(){}

    virtual TextureHandle GetTexture(const char* filename)=0;
    virtual TextureHandle GetTexture(void* rawData, TextureInfo& info)=0;
    virtual void ReleaseTexture(TextureHandle& handle)=0;
};


OOLUA_PROXY_CLASS(TextureHandle)
        OOLUA_TYPEDEFS
                OOLUA::Equal_op,
                OOLUA::Not_equal_op
        OOLUA_END_TYPES
    OOLUA_ONLY_DEFAULT_CONSTRUCTOR
    OOLUA_MEM_FUNC_0(ITexture*, Get)
  //  OOLUA_MEM_FUNC_0_CONST(const ISound*, Get)
OOLUA_CLASS_END

OOLUA_PROXY_CLASS(ITextureManager)
    OOLUA_MEM_FUNC(TextureHandle, GetTexture, const char*)
    OOLUA_MEM_FUNC(void, ReleaseTexture, TextureHandle&)
    OOLUA_TYPEDEFS Abstract OOLUA_END_TYPES
OOLUA_CLASS_END
#endif // ITEXTUREMANAGER_H
