#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "HandleManager.h"
#include "Handle.h"
#include "ITextureManager.h"

class TextureManager : public ITextureManager
{
    TextureHandleManager handleManager;

public:
    TextureManager();
    ~TextureManager(){}

    TextureHandle GetTexture(const char* fileName);
    void ReleaseTexture(TextureHandle&);
};

#endif // TEXTUREMANAGER_H
