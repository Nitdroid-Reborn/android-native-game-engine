#ifndef SPRITEBATCHER_H
#define SPRITEBATCHER_H

#include "ISpriteBatcher.h"
#include <Utils/Profiler.h>
#include "IGeometry.h"
#include <ContentManager/IContentManager.h>

class ShaderProgram;
class Shader;
class VBO;


struct Sprite {
    F32 x;
    F32 y;
    F32 z;
    F32 width;
    F32 height;
    F32 angle;
    TextureRegion texRegion;
    ITexture* texture;
    U8 r;
    U8 g;
    U8 b;
    U8 a;

    Sprite(F32 x, F32 y, F32 z, F32 width, F32 height, F32 angle, const TextureRegion& region,
           ITexture* texture) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->width = width;
        this->height = height;
        this->angle = angle;
        this->texRegion = region;
        this->texture = texture;

        r=g=b=a=255;
    }

    bool operator<(const Sprite& rhs) const{
        if(z == rhs.z) return texture>rhs.texture;
        return z>rhs.z;
    }
};




class SpriteBatcher : public ISpriteBatcher
{
public:
    SpriteBatcher(U16 maxSprites);
    ~SpriteBatcher();

    static ProfilerManager* batcherProfileManager;

    void BeginBatch();
    void EndBatch();
    void DrawSprite(ITexture* texture, F32 x, F32 y, F32 z, F32 width, F32 height,
                    TextureRegion texRegion, F32 angle=0.0f);

    void DrawSprite(U8 r, U8 g, U8 b, U8 a, F32 x, F32 y, F32 z, F32 width, F32 height,
                    F32 angle=0.0f);

    void DrawSprite(ITexture* texture, F32 x, F32 y, F32 z, F32 width, F32 height,
                    TextureRegion texRegion, U8 r, U8 g, U8 b, U8 a, F32 angle=0.0f);

    void SwapSpriteBuffer();

    void Bind();

    void Init();
private:
    U8 vertexSize;
    Vertex3D* vertices;
    U16 numSprites;
    U16 numVertices;
    U32 numIndices;
    U32 verticesIndex;
    U16* indices;


    vector<Sprite> sprites;
    vector<Sprite> oldSprites;

    ShaderProgramHandle shaderProgram;

    VBO* vbo;
};

#endif // SPRITEBATCHER_H
