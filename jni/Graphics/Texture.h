#ifndef TEXTURE_H
#define TEXTURE_H

#include "ITexture.h"

class Texture : public ITexture
{
public:
    Texture(const char *filename);
    ~Texture();

    bool Load();
    bool Reload();
    void SetFilters(U32 minFilter, U32 magFilter);
    void Bind();
    void Dispose();

//private:
    U32 textureId;
};

#endif // TEXTURE_H
