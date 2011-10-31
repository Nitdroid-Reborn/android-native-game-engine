#include "VBO.h"


VBO::VBO() {
    glGenBuffers(1, &verticesId);
    glBindBuffer(GL_ARRAY_BUFFER, verticesId);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D)*geometry->GetVerticesCount(), &geometry->GetVertices()[0].position[0], GL_STATIC_DRAW);

    glGenBuffers(1, &indicesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(U16)*geometry->GetIndicesCount(), &geometry->GetIndices()[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VBO::VBO(IGeometry *geometry) {
    glGenBuffers(1, &verticesId);
    glBindBuffer(GL_ARRAY_BUFFER, verticesId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D)*geometry->GetVerticesCount(), &geometry->GetVertices()[0].position[0], GL_STATIC_DRAW);

    glGenBuffers(1, &indicesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(U16)*geometry->GetIndicesCount(), &geometry->GetIndices()[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void VBO::SetData(int numVertices, Vertex3D *vertices, int numIndices, U16 *indices) {
    glBindBuffer(GL_ARRAY_BUFFER, verticesId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D)*numVertices, &vertices[0].position[0], GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(U16)*numIndices, &indices[0], GL_STREAM_DRAW);
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, verticesId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
}

void VBO::Release() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VBO::~VBO() {
    glDeleteBuffers(1, &verticesId);
    glDeleteBuffers(1, &indicesId);
}

int VBO::GetVertexAttributeSize(VertexAttribute attr) {
    switch(attr) {
        case VertexPosition:
            return 3;
        case VertexTexCoord:
            return 2;
        case VertexNormal:
            return 3;
        case VertexColor:
            return 4;
    }
}

GLenum VBO::GetVertexAttributeType(VertexAttribute attr) {
    switch(attr) {
        case VertexPosition:
            return GL_FLOAT;
        case VertexTexCoord:
            return GL_FLOAT;
        case VertexNormal:
            return GL_FLOAT;
        case VertexColor:
            return GL_UNSIGNED_BYTE;
    }
}

bool VBO::GetVertexAttributeNormalization(VertexAttribute attr) {
    if(attr==VertexColor)
        return true;
    return false;
}

int VBO::GetVertexAttributeStride(VertexAttribute attr) {
    return sizeof(Vertex3D);
}

char* VBO::GetVertexAttributeOffset(VertexAttribute attr) {
    switch(attr) {
        case VertexPosition:
            return BUFFER_OFFSET(0);
        case VertexNormal:
            return BUFFER_OFFSET(12);
        case VertexTexCoord:
            return BUFFER_OFFSET(24);
        case VertexColor:
            return BUFFER_OFFSET(32);
    }
}

void VBO::Draw(int start, int count) {
//    glDrawElements(GL_TRIANGLES, count*3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(start*sizeof(U16)));
}
