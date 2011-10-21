#include "TextureManager.h"
#include <Graphics/OGLTextureFactory.h>


TextureManager::TextureManager() : ITextureManager()
{
    TextureHandle::manager = &handleManager;
    handleManager.SetDataFactory(new OGLTextureFactory());
    loadedTextures.clear();
}

TextureManager::~TextureManager() {
    loadedTextures.clear();
}

TextureHandle TextureManager::GetTexture(const char *fileName) {

    if(loadedTextures.count(fileName)!=0) {
        return loadedTextures[fileName];
    }

    TextureHandle handle;
    ITexture* tex = handleManager.Acquire(handle);
    tex->Load(fileName);

    loadedTextures[fileName] = handle;
    return handle;
}

TextureHandle TextureManager::GetTexture(void* rawData, TextureInfo& info) {
    TextureHandle handle;
    ITexture* tex = handleManager.Acquire(handle);
    tex->Load(rawData, info);

    return handle;
}

void TextureManager::ReleaseTexture(TextureHandle& handle) {
    handleManager.Release(handle);
}
