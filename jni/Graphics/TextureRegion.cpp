#include "TextureRegion.h"

TextureRegion::TextureRegion(float _u1, float _v1, float _u2, float _v2) :
    u1(_u1), v1(_v1), u2(_u2), v2(_v2) {
}


TextureRegion::TextureRegion(float _u1, float _v1, float _u2, float _v2, const TextureHandle* _texture) :
    u1(_u1), v1(_v1), u2(_u2), v2(_v2), texture(_texture) {
}
