#include "OGLTexture.h"

#ifdef ANDROID
#include <GLES/gl.h>
#else
#include <GL/gl.h>
#endif

#include "../ContentManager/PNGLoader.h"

OGLTexture::OGLTexture() : ITexture()
{
    textureInfo.format=TextureInfo::RGB8;
    textureInfo.width=0;
    textureInfo.height=0;
    textureInfo.minFilter=0;
    textureInfo.magFilter=0;
    textureId=0;
}

OGLTexture::~OGLTexture() {
    if(textureId!=0)
        Dispose();
}


bool OGLTexture::Load(const char *filename) {
    this->filename = filename;
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
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

            textureInfo.width=w;
            textureInfo.height=h;
            textureInfo.minFilter=GL_LINEAR;
            textureInfo.magFilter=GL_LINEAR;

            glBindTexture(GL_TEXTURE_2D, 0);
            delete[] imageData;
            return true;
        }
        delete[] imageData;
        return false;
    }

    return false;

}


bool OGLTexture::Load(void* rawData, TextureInfo& info) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, info.pixelAlignment);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if(info.format == TextureInfo::LUMINANCE_ALPHA) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, info.width, info.height, 0,
                     GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, rawData);
    }
    return false;

    return true;
}

bool OGLTexture::Reload() {
    bool res = Load(filename);
    if(!res) return false;
    glBindTexture(GL_TEXTURE_2D, textureId);
    SetFilters(textureInfo.minFilter, textureInfo.magFilter);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void OGLTexture::Dispose() {
    glBindTexture(GL_TEXTURE_2D, textureId);
    unsigned int textures[] = {textureId};
    glDeleteTextures(1, textures);
    textureId=0;
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OGLTexture::Bind() {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void OGLTexture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OGLTexture::SetFilters(U32 minFilter, U32 magFilter) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );
    textureInfo.minFilter=minFilter;
    textureInfo.magFilter=magFilter;
}

//EXPORT_OOLUA_NO_FUNCTIONS(ITexture)
