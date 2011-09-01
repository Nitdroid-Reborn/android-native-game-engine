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
    virtual void ReleaseTexture(TextureHandle& handle)=0;
};

#endif // ITEXTUREMANAGER_H
