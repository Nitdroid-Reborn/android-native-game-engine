#ifndef TEXTUREREGION_H
#define TEXTUREREGION_H

#include <ContentManager/TextureManager.h>

class TextureRegion
{
public:
    TextureRegion();
    TextureRegion(float u1, float v1, float u2, float v2);

    void ConvertToOES(ITexture*);
    float u1;
    float v1;
    float u2;
    float v2;
};
#endif // TEXTUREREGION_H
