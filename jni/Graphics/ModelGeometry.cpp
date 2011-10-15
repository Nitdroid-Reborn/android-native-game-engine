#include "ModelGeometry.h"
#include "MS3DModel.h"
#include <Utils/Log.h>

ModelGeometry::ModelGeometry()
{
    indices=NULL;
    indicesCount=0;
    vertices=NULL;
    verticesCount=0;
}

ModelGeometry::~ModelGeometry() {
    if(verticesCount)
        delete[] vertices;
    if(indicesCount)
        delete[] indices;
}


void ModelGeometry::Load(const char *filename) {
    MS3DModel model;
    model.Load(filename, vertices, verticesCount,
               indices, indicesCount);

    Logger::Log("%p %p", vertices, indices);
}
