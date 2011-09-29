#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "HandleManager.h"
#include "Handle.h"
#include "ITextureManager.h"
#include <map>
#include <string>

//! Implementation of texture manager
class TextureManager : public ITextureManager
{
    TextureHandleManager handleManager;

public:
    TextureManager();
    ~TextureManager();

    TextureHandle GetTexture(const char* fileName);
    TextureHandle GetTexture(void* rawData, TextureInfo& info);
    void ReleaseTexture(TextureHandle&);

private:
    std::map<std::string, TextureHandle> loadedTextures;
};

#endif // TEXTUREMANAGER_H
