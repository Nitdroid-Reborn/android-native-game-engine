#ifndef TEXTUREREGION_H
#define TEXTUREREGION_H

#include <ContentManager/TextureManager.h>

class TextureRegion
{
public:
    TextureRegion(float u1, float v1, float u2, float v2);
    TextureRegion(float u1, float v1, float u2, float v2, const TextureHandle* texture);

    const float u1;
    const float v1;
    const float u2;
    const float v2;
    const TextureHandle* texture;
};

#endif // TEXTUREREGION_H
