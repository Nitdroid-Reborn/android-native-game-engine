#ifndef MODELGEOMETRY_H
#define MODELGEOMETRY_H
#include "IGeometry.h"
#include <vector>
#include "ContentManager/ITextureManager.h"

class VBO;
class ShaderProgram;


struct Mesh {
    U16 startIndex;
    U16 indicesCount;
    char materialIndex;
};

struct Material {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emissive[4];
    float shinniness;
    float transparency;
    TextureHandle texture;
};

class ModelGeometry : public IGeometry
{
public:
    ModelGeometry();
    ~ModelGeometry();

    void Load(const char* filename);

    int GetMeshCount() {return meshesCount;}
    Material* GetMaterial(int mesh);

    int GetVerticesCount() {return verticesCount;}
    int GetIndicesCount() {return indicesCount;}

    Vertex3D* GetVertices() {return vertices;}
    U16* GetIndices() {return indices;}

    U16 GetMeshStartIndex(int mesh);
    U16 GetMeshIndicesCount(int mesh);

    void Draw(Camera* camera, const Matrix4x4& worldMatrix, ShaderProgram* shaderProgram,
              const ShaderParametersList& shaderParameters);

private:
    VBO* vbo;
    Mesh* meshes;
    Material* materials;

    int meshesCount;
    int materialsCount;

    Vertex3D* vertices;
    U16* indices;
    int verticesCount;
    int indicesCount;
};

#endif // MODELGEOMETRY_H
