#ifndef ITEXTUREFACTORY_H
#define ITEXTUREFACTORY_H

#include <Graphics/ITexture.h>
class ITextureFactory {
public:
    ITextureFactory(){}
    virtual ITexture* Create()=0;
};

#endif // ITEXTUREFACTORY_H
