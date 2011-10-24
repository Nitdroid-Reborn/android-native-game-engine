#include "SpriteBatcher.h"
#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "GLee.h"

#endif

#include <Math/MathLib.h>
#include <cmath>
#include "ShaderProgram.h"
#include "Shader.h"
#include <ContentManager/IContentManager.h>

#define PI 3.14159265358979323846
ProfilerManager* SpriteBatcher::batcherProfileManager;



SpriteBatcher::SpriteBatcher(U16 maxSprites) : ISpriteBatcher(maxSprites) {
    indices = new U16[maxSprites*6];
    vertices = new Vertex3D[maxSprites*4]; //each sprite 4 vertices

    verticesIndex=0;
    numVertices = maxSprites*4;
    numIndices = maxSprites*6;


    numSprites=0;

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


void SpriteBatcher::SwapSpriteBuffer() {
    oldSprites = sprites;
    sprites.clear();
}

SpriteBatcher::~SpriteBatcher() {
    delete[] vertices;
    vertices = NULL;
    delete[] indices;
    indices = NULL;

    IContentManager::get()->GetShaderProgramManager()->ReleaseShaderProgram(shaderProgram);

   /* if(shaderProgram) {
        delete shaderProgram;
    }
    if(vertexShader)
        delete vertexShader;
    if(pixelShader)
        delete pixelShader;
*/
    if(vbo)
        delete vbo;
}

void SpriteBatcher::BeginBatch() {
    numSprites=0;
    verticesIndex=0;
}

void SpriteBatcher::Bind() {
    vbo->Bind();
}


void SpriteBatcher::EndBatch() {

    int texChanges = 0;

    {
        PROFILE("Sprites sorting", batcherProfileManager);
        sort(oldSprites.begin(), oldSprites.end());
    }


    vbo->Bind();
    ShaderProgram* sp = shaderProgram.Get();
    sp->Bind();
    sp->EnableAttributeArray("vPosition");
    sp->EnableAttributeArray("vTexCoords");
    sp->EnableAttributeArray("vColor");


    ITexture* currentTexture = 0;
    vector<Sprite>::iterator it = oldSprites.begin();


    Matrix4x4 mat;
    mat.SetOrtho(0.0, 800, 0.0, 480, -1.0, 10.0);


    sp->SetUniformValue("modelViewProjectionMatrix", mat);
    sp->SetUniformValue("textureSampler", 0);

    for(it; it!=oldSprites.end(); ++it) {

        if(currentTexture != (*it).texture) {


            vbo->SetData((int)verticesIndex, &vertices[0],
                         (int)numIndices, &indices[0]);


            sp->SetAttributeArray(vbo);

            vbo->Draw(0, numSprites*2);
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

            vertices[verticesIndex].position[0] = x1;
            vertices[verticesIndex].position[1] = y1;
            vertices[verticesIndex].position[2] = 0;
            vertices[verticesIndex].texCoords[0] = (*it).texRegion.u1;
            vertices[verticesIndex].texCoords[1] = (*it).texRegion.v1;
            vertices[verticesIndex].color[0] = (*it).r;
            vertices[verticesIndex].color[1] = (*it).g;
            vertices[verticesIndex].color[2] = (*it).b;
            vertices[verticesIndex++].color[3] = (*it).a;


            vertices[verticesIndex].position[0] = x2;
            vertices[verticesIndex].position[1] = y2;
            vertices[verticesIndex].position[2] = 0;
            vertices[verticesIndex].texCoords[0] = (*it).texRegion.u2;
            vertices[verticesIndex].texCoords[1] = (*it).texRegion.v1;
            vertices[verticesIndex].color[0] = (*it).r;
            vertices[verticesIndex].color[1] = (*it).g;
            vertices[verticesIndex].color[2] = (*it).b;
            vertices[verticesIndex++].color[3] = (*it).a;


            vertices[verticesIndex].position[0] = x3;
            vertices[verticesIndex].position[1] = y3;
            vertices[verticesIndex].position[2] = 0;
            vertices[verticesIndex].texCoords[0] = (*it).texRegion.u2;
            vertices[verticesIndex].texCoords[1] = (*it).texRegion.v2;
            vertices[verticesIndex].color[0] = (*it).r;
            vertices[verticesIndex].color[1] = (*it).g;
            vertices[verticesIndex].color[2] = (*it).b;
            vertices[verticesIndex++].color[3] = (*it).a;


            vertices[verticesIndex].position[0] = x4;
            vertices[verticesIndex].position[1] = y4;
            vertices[verticesIndex].position[2] = 0;
            vertices[verticesIndex].texCoords[0] = (*it).texRegion.u1;
            vertices[verticesIndex].texCoords[1] = (*it).texRegion.v2;
            vertices[verticesIndex].color[0] = (*it).r;
            vertices[verticesIndex].color[1] = (*it).g;
            vertices[verticesIndex].color[2] = (*it).b;
            vertices[verticesIndex++].color[3] = (*it).a;

            numSprites++;

        }
        else {
            float x1 = (*it).x - halfWidth;
            float y1 = (*it).y - halfHeight;
            float x2 = (*it).x + halfWidth;
            float y2 = (*it).y + halfHeight;

            vertices[verticesIndex].position[0] = x1;
            vertices[verticesIndex].position[1] = y1;
            vertices[verticesIndex].position[2] = 0;
            vertices[verticesIndex].texCoords[0] = (*it).texRegion.u1;
            vertices[verticesIndex].texCoords[1] = (*it).texRegion.v1;
            vertices[verticesIndex].color[0] = (*it).r;
            vertices[verticesIndex].color[1] = (*it).g;
            vertices[verticesIndex].color[2] = (*it).b;
            vertices[verticesIndex++].color[3] = (*it).a;

            vertices[verticesIndex].position[0] = x2;
            vertices[verticesIndex].position[1] = y1;
            vertices[verticesIndex].position[2] = 0;
            vertices[verticesIndex].texCoords[0] = (*it).texRegion.u2;
            vertices[verticesIndex].texCoords[1] = (*it).texRegion.v1;
            vertices[verticesIndex].color[0] = (*it).r;
            vertices[verticesIndex].color[1] = (*it).g;
            vertices[verticesIndex].color[2] = (*it).b;
            vertices[verticesIndex++].color[3] = (*it).a;

            vertices[verticesIndex].position[0] = x2;
            vertices[verticesIndex].position[1] = y2;
            vertices[verticesIndex].position[2] = 0;
            vertices[verticesIndex].texCoords[0] = (*it).texRegion.u2;
            vertices[verticesIndex].texCoords[1] = (*it).texRegion.v2;
            vertices[verticesIndex].color[0] = (*it).r;
            vertices[verticesIndex].color[1] = (*it).g;
            vertices[verticesIndex].color[2] = (*it).b;
            vertices[verticesIndex++].color[3] = (*it).a;

            vertices[verticesIndex].position[0] = x1;
            vertices[verticesIndex].position[1] = y2;
            vertices[verticesIndex].position[2] = 0;
            vertices[verticesIndex].texCoords[0] = (*it).texRegion.u1;
            vertices[verticesIndex].texCoords[1] = (*it).texRegion.v2;
            vertices[verticesIndex].color[0] = (*it).r;
            vertices[verticesIndex].color[1] = (*it).g;
            vertices[verticesIndex].color[2] = (*it).b;
            vertices[verticesIndex++].color[3] = (*it).a;

            numSprites++;
        }
    }

    vbo->SetData(verticesIndex, &vertices[0],
                 numIndices, &indices[0]);


    sp->SetAttributeArray(vbo);

    vbo->Draw(0, numSprites*2);


    if(currentTexture) {
        currentTexture->Unbind();
        currentTexture = 0;
    }

    sp->Release();
    vbo->Release();
}

void SpriteBatcher::Init() {
    shaderProgram = IContentManager::get()->GetShaderProgramManager()->GetShaderProgram("spriteBatcher");

    vbo = new VBO();
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
