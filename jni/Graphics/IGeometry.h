#ifndef IGEOMETRY_H
#define IGEOMETRY_H
#include <Utils/EngineTypes.h>
#include <Math/MathLib.h>

class VBO;
class ShaderProgram;
class Camera;


struct Vertex3D  {
    float position[3];
    float normal[3];
    float texCoords[2];
    unsigned char color[4];
};


class IGeometry {

public:
    IGeometry(){}
    virtual ~IGeometry(){}

    virtual int GetVerticesCount()=0;
    virtual int GetIndicesCount()=0;

    virtual Vertex3D* GetVertices()=0;
    virtual U16* GetIndices()=0;

    virtual void Draw(Camera* camera, const Matrix4x4& worldMatrix, ShaderProgram* shaderProgram)=0;
};

#endif // IGEOMETRY_H
