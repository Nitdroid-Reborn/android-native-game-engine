#include "OGLTextureFactory.h"
#include <Graphics/OGLTexture.h>

OGLTextureFactory::OGLTextureFactory()
{
}

ITexture* OGLTextureFactory::Create() {
    return new OGLTexture();
}
