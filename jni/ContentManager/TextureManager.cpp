#include "TextureManager.h"
#include <Graphics/OGLTextureFactory.h>


TextureManager::TextureManager() : ITextureManager()
{
    TextureHandle::manager = &handleManager;
    handleManager.SetDataFactory(new OGLTextureFactory());
}

TextureHandle TextureManager::GetTexture(const char *fileName) {
    TextureHandle handle;
    ITexture* tex = handleManager.Acquire(handle);
    tex->Load(fileName);

    return handle;
}

void TextureManager::ReleaseTexture(TextureHandle& handle) {
    handleManager.Release(handle);
}
