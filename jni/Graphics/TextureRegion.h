#ifndef TEXTUREREGION_H
#define TEXTUREREGION_H

#include <ContentManager/TextureManager.h>

class TextureRegion
{
public:
    TextureRegion();
    TextureRegion(float u1, float v1, float u2, float v2);
    float u1;
    float v1;
    float u2;
    float v2;
};

/*OOLUA_PROXY_CLASS(TextureRegion)
    OOLUA_NO_TYPEDEFS
    OOLUA_CONSTRUCTORS_BEGIN
                    OOLUA_CONSTRUCTOR_4(float, float, float, float)
    OOLUA_CONSTRUCTORS_END
OOLUA_CLASS_END*/

#endif // TEXTUREREGION_H
