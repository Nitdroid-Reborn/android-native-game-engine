#ifndef ISPRITEBATCHER_H
#define ISPRITEBATCHER_H


#include "ITexture.h"
#include "TextureRegion.h"

class ISpriteBatcher {
public:
    ISpriteBatcher(U16 _maxSprites) : maxSprites(_maxSprites){}
    virtual ~ISpriteBatcher(){}

    virtual void BeginBatch(ITexture* texture)=0;
    virtual void EndBatch()=0;
    virtual void DrawSprite(F32 x, F32 y, F32 width, F32 height,
                            TextureRegion texRegion, F32 angle=0.0f)=0;

protected:
    U16 maxSprites;
};

#endif // ISPRITEBATCHER_H
