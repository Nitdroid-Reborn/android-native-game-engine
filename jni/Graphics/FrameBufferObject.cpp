#include "FrameBufferObject.h"
#include <Utils/Log.h>

FrameBufferObject::FrameBufferObject(int width, int height, GLenum internalForma, GLenum format, GLenum type)
{
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, internalForma, width, height, 0,
                 format, type, 0);
    Logger::Log("%d", glGetError());
    glBindTexture(GL_TEXTURE_2D, 0);

    // create a renderbuffer object to store depth info
    glGenRenderbuffers(1, &rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
                             width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // create a framebuffer object
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, textureId, 0);
    Logger::Log("%d", glGetError());

    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 GL_RENDERBUFFER, rboId);
    Logger::Log("%d", glGetError());


    Logger::Log("%d", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    // switch back to window-system-provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBufferObject::~FrameBufferObject() {
    glDeleteRenderbuffers(1, &rboId);
    glDeleteFramebuffers(1, &fboId);
    glDeleteTextures(1, &textureId);
}

void FrameBufferObject::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    //Logger::Log("%d", glGetError());
}

void FrameBufferObject::Release() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBufferObject::Texture() const {
    return textureId;
}
