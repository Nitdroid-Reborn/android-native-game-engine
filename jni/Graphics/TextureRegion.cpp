#include "TextureRegion.h"


TextureRegion::TextureRegion() {
    u1=v1=0.0;
    u2=v2=1.0;
}

TextureRegion::TextureRegion(float _u1, float _v1, float _u2, float _v2) :
    u1(_u1), v1(_v1), u2(_u2), v2(_v2) {
}

EXPORT_OOLUA_NO_FUNCTIONS(TextureRegion)
