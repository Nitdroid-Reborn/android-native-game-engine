#include "SpriteBatcher.h"
#include <GLES/gl.h>
#include <cmath>

#define PI 3.14159265358979323846
SpriteBatcher::SpriteBatcher(U16 maxSprites) : ISpriteBatcher(maxSprites) {
    indices = new U16[maxSprites*6];
    vertices = new F32[maxSprites*4*4]; //each sprite 4 vertices: x, y, tu, tv

    verticesIndex=0;
    numVertices = maxSprites*4*4;
    numIndices = maxSprites*6;

    vertexSize = 8*4;


    U16 j=0;
    for(int i=0;i<numIndices; i+=6, j+=4) {
        indices[i+0] = j+0;
        indices[i+1] = j+1;
        indices[i+2] = j+2;
        indices[i+3] = j+2;
        indices[i+4] = j+3;
        indices[i+5] = j+0;
    }
}

SpriteBatcher::~SpriteBatcher() {
    delete[] vertices;
    vertices = NULL;
    delete[] indices;
    indices = NULL;
}

void SpriteBatcher::BeginBatch(ITexture *texture) {
    if(texture!=0)
        texture->Bind();
    numSprites=0;
    verticesIndex=0;
}

void SpriteBatcher::EndBatch() {


    glVertexPointer(2, GL_FLOAT, sizeof(F32)*4, vertices);
    glTexCoordPointer(2, GL_FLOAT, sizeof(F32)*4, &vertices[2]);


    glDrawElements(GL_TRIANGLES, numSprites*6, GL_UNSIGNED_SHORT, indices);

    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisableClientState(GL_VERTEX_ARRAY);
}

void SpriteBatcher::DrawSprite(F32 x, F32 y, F32 width, F32 height,
                               TextureRegion texRegion, F32 angle) {
    float halfWidth = width/2;
    float halfHeight = height/2;

    if(angle!=0.0f) {
        float radians = angle*PI/180.0f;
        float angle_cos = cos(radians);
        float angle_sin = sin(radians);


        float x1 = -halfWidth * angle_cos - (-halfHeight) * angle_sin;
        float y1 = -halfWidth * angle_sin + (-halfHeight) * angle_cos;
        float x2 = -halfWidth * angle_cos - (halfHeight) * angle_sin;
        float y2 = -halfWidth * angle_sin + (halfHeight) * angle_cos;
        float x3 = halfWidth * angle_cos - halfHeight * angle_sin;
        float y3 = halfWidth * angle_sin + halfHeight * angle_cos;
        float x4 = halfWidth * angle_cos - (-halfHeight) * angle_sin;
        float y4 = halfWidth * angle_sin + (-halfHeight) * angle_cos;

        x1 += x;
        y1 += y;
        x2 += x;
        y2 += y;
        x3 += x;
        y3 += y;
        x4 += x;
        y4 += y;

        vertices[verticesIndex++] = x1;
        vertices[verticesIndex++] = y1;
        vertices[verticesIndex++] = texRegion.u1;
        vertices[verticesIndex++] = texRegion.v2;

        vertices[verticesIndex++] = x2;
        vertices[verticesIndex++] = y2;
        vertices[verticesIndex++] = texRegion.u1;
        vertices[verticesIndex++] = texRegion.v1;

        vertices[verticesIndex++] = x3;
        vertices[verticesIndex++] = y3;
        vertices[verticesIndex++] = texRegion.u2;
        vertices[verticesIndex++] = texRegion.v1;

        vertices[verticesIndex++] = x4;
        vertices[verticesIndex++] = y4;
        vertices[verticesIndex++] = texRegion.u2;
        vertices[verticesIndex++] = texRegion.v2;

        numSprites++;

    }
    else {
        float x1 = x - halfWidth;
        float y1 = y - halfHeight;
        float x2 = x + halfWidth;
        float y2 = y + halfHeight;

        vertices[verticesIndex++] = x1;
        vertices[verticesIndex++] = y1;
        vertices[verticesIndex++] = texRegion.u1;
        vertices[verticesIndex++] = texRegion.v2;

        vertices[verticesIndex++] = x1;
        vertices[verticesIndex++] = y2;
        vertices[verticesIndex++] = texRegion.u1;
        vertices[verticesIndex++] = texRegion.v1;

        vertices[verticesIndex++] = x2;
        vertices[verticesIndex++] = y2;
        vertices[verticesIndex++] = texRegion.u2;
        vertices[verticesIndex++] = texRegion.v1;

        vertices[verticesIndex++] = x2;
        vertices[verticesIndex++] = y1;
        vertices[verticesIndex++] = texRegion.u2;
        vertices[verticesIndex++] = texRegion.v2;

        numSprites++;
    }
}
