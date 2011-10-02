#include "SpriteBatcher.h"
#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "GLee.h"
#include <GL/gl.h>

#endif

#include <Math/MathLib.h>
#include <cmath>

#define PI 3.14159265358979323846
ProfilerManager* SpriteBatcher::batcherProfileManager;

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
    "attribute vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = mvp * (vPosition);\n"
    "}\n";

static const char gFragmentShader[] =
    "precision mediump float;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
    "}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

SpriteBatcher::SpriteBatcher(U16 maxSprites) : ISpriteBatcher(maxSprites) {
    indices = new U16[maxSprites*6];
    vertices = new Vertex[maxSprites*4]; //each sprite 4 vertices

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


    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, 0, GL_STREAM_DRAW);


    glGenBuffers(1, &indicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(U16)*numIndices, &indices[0], GL_STATIC_DRAW);

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

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indicesBuffer);
}

void SpriteBatcher::BeginBatch() {
    numSprites=0;
    verticesIndex=0;
}

void SpriteBatcher::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
}

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
void SpriteBatcher::EndBatch() {

    int texChanges = 0;

    {
        PROFILE("Sprites sorting", batcherProfileManager);
        sort(oldSprites.begin(), oldSprites.end());
    }

    glUseProgram(gProgram);
    checkGlError("glUseProgram");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");

   // glPushMatrix();

   // glScalef(0.1, 0.1, 0.1);

   // glMatrixMode(GL_TEXTURE);
   // glPushMatrix();
   // glScalef(0.001, 0.001, 0.001);



    ITexture* currentTexture = 0;
    vector<Sprite>::iterator it = oldSprites.begin();


    Matrix4x4 mat;
    mat.SetOrtho(0.0, 800, 0.0, 480, -1.0, 1.0);

    Matrix4x4 scale;
    scale.SetScale(Vector3(0.1 ,0.1, 0.1));

    mat = mat*scale;



    glUniformMatrix4fv(gvMatrixHandle, 1, 0, &mat.entries[0]);
    checkGlError("glUniformMatrix4fv");
    //LOGI("%d", oldSprites.size());
    for(it; it!=oldSprites.end(); ++it) {

        if(currentTexture != (*it).texture) {



            //glVertexAttribPointer(gvPositionHandle, 2, GL_SHORT, GL_FALSE, sizeof(Vertex), &vertices[0].x);




            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)*verticesIndex, &vertices[0].x);
            glVertexAttribPointer(gvPositionHandle, 2, GL_SHORT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
            //glVertexPointer(2, GL_SHORT, sizeof(Vertex), BUFFER_OFFSET(0));
            //glTexCoordPointer(2, GL_SHORT, sizeof(Vertex), BUFFER_OFFSET(4));
            //glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), BUFFER_OFFSET(8));

            glDrawElements(GL_TRIANGLES, numSprites*6, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));


           /* glVertexPointer(2, GL_SHORT, sizeof(Vertex), &vertices[0].x);
            glTexCoordPointer(2, GL_SHORT, sizeof(Vertex), &vertices[0].u);
            glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &vertices[0].r);*/

          //  glDrawElements(GL_TRIANGLES, numSprites*6, GL_UNSIGNED_SHORT, indices);
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

            vertices[verticesIndex].x = (I16)x1*10;
            vertices[verticesIndex].y = (I16)y1*10;
            vertices[verticesIndex].u = (I16)((*it).texRegion.u1*1000);
            vertices[verticesIndex].v = (I16)((*it).texRegion.v1*1000);
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;


            vertices[verticesIndex].x = (I16)x2*10;
            vertices[verticesIndex].y = (I16)y2*10;
            vertices[verticesIndex].u = (I16)((*it).texRegion.u2*1000);
            vertices[verticesIndex].v = (I16)((*it).texRegion.v1*1000);
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;


            vertices[verticesIndex].x = (I16)x3*10;
            vertices[verticesIndex].y = (I16)y3*10;
            vertices[verticesIndex].u = (I16)((*it).texRegion.u2*1000);
            vertices[verticesIndex].v = (I16)((*it).texRegion.v2*1000);
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;


            vertices[verticesIndex].x = (I16)x4*10;
            vertices[verticesIndex].y = (I16)y4*10;
            vertices[verticesIndex].u = (I16)((*it).texRegion.u1*1000);
            vertices[verticesIndex].v = (I16)((*it).texRegion.v2*1000);
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

            vertices[verticesIndex].x = (I16)x1*10;
            vertices[verticesIndex].y = (I16)y1*10;
            vertices[verticesIndex].u = (I16)((*it).texRegion.u1*1000);
            vertices[verticesIndex].v = (I16)((*it).texRegion.v1*1000);
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            vertices[verticesIndex].x = (I16)x2*10;
            vertices[verticesIndex].y = (I16)y1*10;
            vertices[verticesIndex].u = (I16)((*it).texRegion.u2*1000);
            vertices[verticesIndex].v = (I16)((*it).texRegion.v1*1000);
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            vertices[verticesIndex].x = (I16)x2*10;
            vertices[verticesIndex].y = (I16)y2*10;
            vertices[verticesIndex].u = (I16)((*it).texRegion.u2*1000);
            vertices[verticesIndex].v = (I16)((*it).texRegion.v2*1000);
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            vertices[verticesIndex].x = (I16)x1*10;
            vertices[verticesIndex].y = (I16)y2*10;
            vertices[verticesIndex].u = (I16)((*it).texRegion.u1*1000);
            vertices[verticesIndex].v = (I16)((*it).texRegion.v2*1000);
            vertices[verticesIndex].r = (*it).r;
            vertices[verticesIndex].g = (*it).g;
            vertices[verticesIndex].b = (*it).b;
            vertices[verticesIndex++].a = (*it).a;

            numSprites++;
        }
    }

   /* glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)*verticesIndex, &vertices[0].x);

    glVertexPointer(2, GL_SHORT, sizeof(Vertex), BUFFER_OFFSET(0));
    glTexCoordPointer(2, GL_SHORT, sizeof(Vertex), BUFFER_OFFSET(4));
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), BUFFER_OFFSET(8));

    glDrawElements(GL_TRIANGLES, numSprites*6, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));*/

    //glVertexAttribPointer(gvPositionHandle, 2, GL_SHORT, GL_FALSE, sizeof(Vertex), &vertices[0].x);
    //glVertexPointer(2, GL_SHORT, sizeof(Vertex), &vertices[0].x);
    //glTexCoordPointer(2, GL_SHORT, sizeof(Vertex), &vertices[0].u);
    //glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &vertices[0].r);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)*verticesIndex, &vertices[0].x);
    glVertexAttribPointer(gvPositionHandle, 2, GL_SHORT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
    //glVertexPointer(2, GL_SHORT, sizeof(Vertex), BUFFER_OFFSET(0));
    //glTexCoordPointer(2, GL_SHORT, sizeof(Vertex), BUFFER_OFFSET(4));
    //glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), BUFFER_OFFSET(8));

    glDrawElements(GL_TRIANGLES, numSprites*6, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));


    //glPopMatrix();
   // glMatrixMode(GL_MODELVIEW);
   // glPopMatrix();

    if(currentTexture) {
        currentTexture->Unbind();
        currentTexture = 0;
    }
}

void SpriteBatcher::Init() {
    gProgram = createProgram(gVertexShader, gFragmentShader);
        if (!gProgram) {
            LOGE("Could not create program.");
            return;
        }
        gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
        gvMatrixHandle = glGetUniformLocation(gProgram, "mvp");
        checkGlError("glGetAttribLocation");
        LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
                gvPositionHandle);

        LOGI("glGetUniformLocation(\"mvp\") = %d\n",
                gvMatrixHandle);
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
