#ifndef TEXTURE_H
#define TEXTURE_H

#include "ITexture.h"

class OGLTexture : public ITexture
{
public:
    OGLTexture();
    ~OGLTexture();

    bool Load(const char *filename);
    bool Load(void* rawData, TextureInfo& info);
    bool Reload();
    void SetFilters(U32 minFilter, U32 magFilter);
    void Bind();
    void Dispose();

//private:
    U32 textureId;
};

#endif // TEXTURE_H
