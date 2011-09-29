#include "TextureRegion.h"


TextureRegion::TextureRegion() {
    u1=v1=0.0;
    u2=v2=1.0;
}

TextureRegion::TextureRegion(float _u1, float _v1, float _u2, float _v2) :
    u1(_u1), v1(_v1), u2(_u2), v2(_v2) {
}


void TextureRegion::ConvertToOES(ITexture* t) {
    u2 = u2 - u1;
    v2 = v2 - v1;

    int w, h;
    w = t->GetInfo().width;
    h = t->GetInfo().height;

    u1*= w;
    u2*= w;
    v1*= h;
    v2*= h;
}
