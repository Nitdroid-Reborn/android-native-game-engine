#ifndef OGLTEXTUREFACTORY_H
#define OGLTEXTUREFACTORY_H

#include "ITextureFactory.h"

class OGLTextureFactory : public ITextureFactory
{
public:
    OGLTextureFactory();

    ITexture* Create();
};

#endif // OGLTEXTUREFACTORY_H
