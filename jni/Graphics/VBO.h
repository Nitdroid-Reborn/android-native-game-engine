#ifndef VBO_H
#define VBO_H
#ifdef ANDROID
#include <GLES2/gl2.h>
#else
#include "GLee.h"
#endif
#include "IGeometry.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))



enum VertexAttribute {
    VertexPosition=0,
    VertexTexCoord,
    VertexNormal,
    VertexColor
};

class VBO
{
public:
    VBO();
    VBO(IGeometry* geometry);
    ~VBO();
    void Bind();
    void Release();

    void SetData(int numVertices, Vertex3D* vertices,
                 int numIndices, U16* indices);

    inline GLuint GetId() const {return indicesId;}

    int GetVertexAttributeSize(VertexAttribute);
    GLenum GetVertexAttributeType(VertexAttribute);
    bool GetVertexAttributeNormalization(VertexAttribute);
    int GetVertexAttributeStride(VertexAttribute);
    char* GetVertexAttributeOffset(VertexAttribute);

    void Draw(int start, int count);


private:
    GLuint verticesId;
    GLuint indicesId;
};

#endif // VBO_H
