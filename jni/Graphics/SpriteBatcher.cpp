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

#define PI 3.14159265358979323846
ProfilerManager* SpriteBatcher::batcherProfileManager;
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}
static const char gVertexShader[] =
    "uniform mat4 mvp;"
    "attribute mediump vec4 vPosition;\n"
    "attribute mediump vec4 vTexCoords;\n"
    "attribute lowp vec4 vColor;\n"
    "varying mediump vec4 texCoords;\n"
    "varying lowp vec4 color;\n"
    "void main() {\n"
    "  gl_Position = mvp * (vPosition);\n"
    "  texCoords = vTexCoords;\n"
    "  color = vColor;\n"
    "}\n";

static const char gFragmentShader[] =
    "uniform sampler2D textureSampler;\n"
    "varying mediump vec4 texCoords;\n"
    "varying lowp vec4 color;\n"
    "void main() {\n"
    "  gl_FragColor = texture2D(textureSampler, texCoords.xy)*color;\n"
    "}\n";

SpriteBatcher::SpriteBatcher(U16 maxSprites) : ISpriteBatcher(maxSprites) {
    indices = new U16[maxSprites*6];
    vertices = new Vertex3D[maxSprites*4]; //each sprite 4 vertices

    verticesIndex=0;
    numVertices = maxSprites*4;
    numIndices = maxSprites*6;

    indicesBuffer=0;
    vertexBuffer=0;


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

    I32 bufferSize;


    /*glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, 0, GL_STREAM_DRAW);


    glGenBuffers(1, &indicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(U16)*numIndices, &indices[0], GL_STATIC_DRAW);
*/
  //  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(U16)*numIndices, &indices[0]);
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

    if(shaderProgram) {
        delete shaderProgram;
    }
    if(vertexShader)
        delete vertexShader;
    if(pixelShader)
        delete pixelShader;

    if(vbo)
        delete vbo;
   // glDeleteBuffers(1, &vertexBuffer);
    //glDeleteBuffers(1, &indicesBuffer);
}

void SpriteBatcher::BeginBatch() {
    numSprites=0;
    verticesIndex=0;
}

void SpriteBatcher::Bind() {
    vbo->Bind();
    //glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
}


void SpriteBatcher::EndBatch() {

    int texChanges = 0;

    {
        PROFILE("Sprites sorting", batcherProfileManager);
        sort(oldSprites.begin(), oldSprites.end());
    }


    vbo->Bind();
    shaderProgram->Bind();
    shaderProgram->EnableAttributeArray("vPosition");
    shaderProgram->EnableAttributeArray("vTexCoords");
    shaderProgram->EnableAttributeArray("vColor");

   // glPushMatrix();

   // glScalef(0.1, 0.1, 0.1);

   // glMatrixMode(GL_TEXTURE);
   // glPushMatrix();
   // glScalef(0.001, 0.001, 0.001);



    ITexture* currentTexture = 0;
    vector<Sprite>::iterator it = oldSprites.begin();


    Matrix4x4 mat;
    mat.SetOrtho(0.0, 800, 0.0, 480, -1.0, 1.0);

   // Matrix4x4 scale;
    //scale.SetScale(Vector3(0.1 ,0.1, 0.1));

    //mat = mat*scale;



    shaderProgram->SetUniformValue("mvp", mat);
    shaderProgram->SetUniformValue("textureSampler", 0);

    for(it; it!=oldSprites.end(); ++it) {

        if(currentTexture != (*it).texture) {


            vbo->SetData((int)verticesIndex, &vertices[0],
                         (int)numIndices, &indices[0]);


            shaderProgram->SetAttributeArray(vbo);

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


    shaderProgram->SetAttributeArray(vbo);

    vbo->Draw(0, numSprites*2);


    if(currentTexture) {
        currentTexture->Unbind();
        currentTexture = 0;
    }

    shaderProgram->Release();
    vbo->Release();
}

void SpriteBatcher::Init() {

    shaderProgram = new ShaderProgram();


    vertexShader = new Shader(Shader::VertexShader);
    vertexShader->CompileSource(std::string(gVertexShader));
    pixelShader = new Shader(Shader::PixelShader);
    pixelShader->CompileSource(std::string(gFragmentShader));

    shaderProgram->AddShader(vertexShader);
    shaderProgram->AddShader(pixelShader);
    shaderProgram->Link();

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
