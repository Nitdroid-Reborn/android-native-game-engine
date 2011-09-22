#include "TextureManager.h"
#include <Graphics/OGLTextureFactory.h>


TextureManager::TextureManager() : ITextureManager()
{
    TextureHandle::manager = &handleManager;
    handleManager.SetDataFactory(new OGLTextureFactory());
    loadedTextures.clear();
}

TextureManager::~TextureManager() {
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

/*EXPORT_OOLUA_FUNCTIONS_NON_CONST(TextureHandle, Get)
EXPORT_OOLUA_FUNCTIONS_CONST(TextureHandle)

EXPORT_OOLUA_FUNCTIONS_NON_CONST(ITextureManager, GetTexture, ReleaseTexture)
EXPORT_OOLUA_FUNCTIONS_CONST(ITextureManager)
*/
