#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H
#ifdef ANDROID
#include <GLES2/gl2.h>
#else
#include "GLee.h"
#endif

class FrameBufferObject
{
public:
    FrameBufferObject(int width, int height, GLenum internalForma, GLenum format, GLenum type);
    ~FrameBufferObject();

    void Bind();
    void Release();

    GLuint Texture() const;

private:
    GLuint textureId;
    GLuint fboId;
    GLuint rboId;
};

#endif // FRAMEBUFFEROBJECT_H

