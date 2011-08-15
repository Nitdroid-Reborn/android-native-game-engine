#include "Texture.h"

#include <GLES/gl.h>
#include "../ContentManager/PNGLoader.h"

Texture::Texture(const char *filename) : ITexture(filename)
{
    textureInfo.format=TextureInfo::RGB8;
    textureInfo.width=0;
    textureInfo.height=0;
    textureInfo.minFilter=0;
    textureInfo.magFilter=0;
    textureId=0;
}

Texture::~Texture() {
    if(textureId!=0)
        Dispose();
}


bool Texture::Load() {
    U8* imageData;
    U16 w, h;
    U8 depth;
    bool alpha;

    PNGLoader loader;

    if(loader.Load(filename, imageData, w, h, depth, alpha)) {


        GLenum format;
        if(depth == 8) {
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);
            if(alpha) {
                format = GL_RGBA;
                textureInfo.format=TextureInfo::RGBA8;
            }
            else {
                format = GL_RGB;
                textureInfo.format=TextureInfo::RGBA8;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format,
                            GL_UNSIGNED_BYTE, imageData);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

            textureInfo.width=w;
            textureInfo.height=h;
            textureInfo.minFilter=GL_NEAREST;
            textureInfo.magFilter=GL_NEAREST;

            glBindTexture(GL_TEXTURE_2D, 0);
            delete[] imageData;
            return true;
        }
        delete[] imageData;
        return false;
    }

    return false;

}

bool Texture::Reload() {
    bool res = Load();
    if(!res) return false;
    glBindTexture(GL_TEXTURE_2D, textureId);
    SetFilters(textureInfo.minFilter, textureInfo.magFilter);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::Dispose() {
    glBindTexture(GL_TEXTURE_2D, textureId);
    unsigned int textures[] = {textureId};
    glDeleteTextures(1, textures);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::SetFilters(U32 minFilter, U32 magFilter) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );
    textureInfo.minFilter=minFilter;
    textureInfo.magFilter=magFilter;
}
