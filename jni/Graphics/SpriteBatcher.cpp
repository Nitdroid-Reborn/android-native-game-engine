#include "SpriteBatcher.h"
#ifdef ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#include <GL/gl.h>
#endif
#include <cmath>

#define PI 3.14159265358979323846
ProfilerManager* SpriteBatcher::batcherProfileManager;

SpriteBatcher::SpriteBatcher(U16 maxSprites) : ISpriteBatcher(maxSprites) {
    indices = new U16[maxSprites*6];
    vertices = new Vertex[maxSprites*4]; //each sprite 4 vertices

    verticesIndex=0;
    numVertices = maxSprites*4;
    numIndices = maxSprites*6;


    numSprites=0;
   // vertexSize = 8*4;


    U16 j=0;
    for(int i=0;i<numIndices; i+=6, j+=4) {
        indices[i+0] = j+0;
        indices[i+1] = j+1;
        indices[i+2] = j+2;
        indices[i+3] = j+2;
        indices[i+4] = j+3;
        indices[i+5] = j+0;
    }

   // oldSprites.reserve(maxSprites);
   // sprites.reserve(maxSprites);
}


void SpriteBatcher::SwapSpriteBuffer() {
    oldSprites = sprites;
    sprites.clear();
}

SpriteBatcher::~SpriteBatcher() {
    delete[] vertices;
    vertices = NULL;
    delete[] indices;
    indices = NULL;
}

void SpriteBatcher::BeginBatch() {
    numSprites=0;
    verticesIndex=0;
}

void SpriteBatcher::EndBatch() {

    int texChanges = 0;

    //Logger::Log("Num sprites: %d", oldSprites.size());

    {
        PROFILE("Sprites sorting", batcherProfileManager);
        sort(oldSprites.begin(), oldSprites.end());
    }


    ITexture* currentTexture = 0;
    vector<Sprite>::iterator it = oldSprites.begin();
    for(it; it!=oldSprites.end(); ++it) {

        if(currentTexture != (*it).texture) {
            glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0]);
            glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0].u);
            glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &vertices[0].r);


            glDrawElements(GL_TRIANGLES, numSprites*6, GL_UNSIGNED_SHORT, indices);

            texChanges++;
            if(!(*it).texture)
                currentTexture->Unbind();

            currentTexture = (*it).texture;
            numSprites=0;
            verticesIndex=0;

            if(currentTexture)
                currentTexture->Bind();
        }

        float halfWidth = (*it).width/2;
        float halfHeight = (*it).height/2;

        if((*it).angle!=0.0f) {
            float radians = (*it).angle*PI/180.0f;
            float angle_cos = cos(radians);
            float angle_sin = sin(radians);


            float x1 = -halfWidth * angle_cos - (-halfHeight) * angle_sin;
            float y1 = -halfWidth * angle_sin + (-halfHeight) * angle_cos;
            float x2 = halfWidth * angle_cos - (-halfHeight) * angle_sin;
            float y2 = halfWidth * angle_sin + (-halfHeight) * angle_cos;
            float x3 = halfWidth * angle_cos - halfHeight * angle_sin;
            float y3 = halfWidth * angle_sin + halfHeight * angle_cos;
            float x4 = -halfWidth * angle_cos - (halfHeight) * angle_sin;
            float y4 = -halfWidth * angle_sin + (halfHeight) * angle_cos;

            x1 += (*it).x;
            y1 += (*it).y;
            x2 += (*it).x;
            y2 += (*it).y;
            x3 += (*it).x;
            y3 += (*it).y;
            x4 += (*it).x;
            y4 += (*it).y;

            vertices[verticesIndex].x = x1;
            vertices[verticesIndex].y = y1;
            vertices[verticesIndex].u = (*it).texRegion.u1;
            vertices[verticesIndex].v = (*it).texRegion.v1;
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            vertices[verticesIndex].x = x2;
            vertices[verticesIndex].y = y2;
            vertices[verticesIndex].u = (*it).texRegion.u2;
            vertices[verticesIndex].v = (*it).texRegion.v1;
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            vertices[verticesIndex].x = x3;
            vertices[verticesIndex].y = y3;
            vertices[verticesIndex].u = (*it).texRegion.u2;
            vertices[verticesIndex].v = (*it).texRegion.v2;
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            vertices[verticesIndex].x = x4;
            vertices[verticesIndex].y = y4;
            vertices[verticesIndex].u = (*it).texRegion.u1;
            vertices[verticesIndex].v = (*it).texRegion.v2;
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            numSprites++;

        }
        else {
            float x1 = (*it).x - halfWidth;
            float y1 = (*it).y - halfHeight;
            float x2 = (*it).x + halfWidth;
            float y2 = (*it).y + halfHeight;

            vertices[verticesIndex].x = x1;
            vertices[verticesIndex].y = y1;
            vertices[verticesIndex].u = (*it).texRegion.u1;
            vertices[verticesIndex].v = (*it).texRegion.v1;
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            vertices[verticesIndex].x = x2;
            vertices[verticesIndex].y = y1;
            vertices[verticesIndex].u = (*it).texRegion.u2;
            vertices[verticesIndex].v = (*it).texRegion.v1;
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            vertices[verticesIndex].x = x2;
            vertices[verticesIndex].y = y2;
            vertices[verticesIndex].u = (*it).texRegion.u2;
            vertices[verticesIndex].v = (*it).texRegion.v2;
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            vertices[verticesIndex].x = x1;
            vertices[verticesIndex].y = y2;
            vertices[verticesIndex].u = (*it).texRegion.u1;
            vertices[verticesIndex].v = (*it).texRegion.v2;
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            numSprites++;
        }
    }

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0].x);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0].u);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &vertices[0].r);

    glDrawElements(GL_TRIANGLES, numSprites*6, GL_UNSIGNED_SHORT, indices);
}

void SpriteBatcher::DrawSprite(ITexture* texture, F32 x, F32 y, F32 z,
                               F32 width, F32 height,
                               TextureRegion texRegion, F32 angle) {

    Sprite s(x, y, z, width, height, angle, texRegion, texture);
    sprites.push_back(s);
}

void SpriteBatcher::DrawSprite(U8 r, U8 g, U8 b, U8 a, F32 x, F32 y, F32 z,
                               F32 width, F32 height, F32 angle) {

    Sprite s(x, y, z, width, height, angle, TextureRegion(), 0);
    s.r = r;
    s.g = g;
    s.b = b;
    s.a = a;
    sprites.push_back(s);
}
