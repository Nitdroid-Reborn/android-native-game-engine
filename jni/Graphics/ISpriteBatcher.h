#ifndef ISPRITEBATCHER_H
#define ISPRITEBATCHER_H


#include "ITexture.h"
#include "TextureRegion.h"

class ISpriteBatcher {
public:
    ISpriteBatcher(U16 _maxSprites) : maxSprites(_maxSprites){}
    virtual ~ISpriteBatcher(){}

    virtual void BeginBatch()=0;
    virtual void EndBatch()=0;
    virtual void DrawSprite(ITexture* texture, F32 x, F32 y, F32 z, F32 width, F32 height,
                            TextureRegion texRegion, F32 angle=0.0f)=0;

    virtual void DrawSprite(U8 r, U8 g, U8 b, U8 a, F32 x, F32 y, F32 z, F32 width, F32 height,
                            F32 angle=0.0f)=0;

  /*  virtual void DrawSprite(Vector3* color, F32 x, F32 y, F32 width, F32 height,
                            TextureRegion texRegion, F32 angle=0.0f)=0;
    */
    virtual void SwapSpriteBuffer()=0;

protected:
    U16 maxSprites;
};

#endif // ISPRITEBATCHER_H
