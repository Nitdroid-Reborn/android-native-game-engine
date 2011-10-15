#ifndef MODELGEOMETRY_H
#define MODELGEOMETRY_H
#include "IGeometry.h"

class ModelGeometry : public IGeometry
{
public:
    ModelGeometry();
    ~ModelGeometry();

    void Load(const char* filename);

    int GetVerticesCount() {return verticesCount;}
    int GetIndicesCount() {return indicesCount;}

    Vertex3D* GetVertices() {return vertices;}
    U16* GetIndices() {return indices;}

private:
    Vertex3D* vertices;
    U16* indices;
    int verticesCount;
    int indicesCount;
};

#endif // MODELGEOMETRY_H
