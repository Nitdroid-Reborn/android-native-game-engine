#ifndef SPRITEBATCHER_H
#define SPRITEBATCHER_H

#include "ISpriteBatcher.h"

class SpriteBatcher : public ISpriteBatcher
{
public:
    SpriteBatcher(U16 maxSprites);
    ~SpriteBatcher();

    void BeginBatch(ITexture *texture);
    void EndBatch();
    void DrawSprite(F32 x, F32 y, F32 width, F32 height, TextureRegion texRegion, F32 angle);

private:
    U8 vertexSize;
    F32* vertices;
    U16 numSprites;
    U16 numVertices;
    U32 numIndices;
    U32 verticesIndex;
    U16* indices;
};

#endif // SPRITEBATCHER_H
