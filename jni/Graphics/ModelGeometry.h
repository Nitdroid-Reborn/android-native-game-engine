#ifndef MODELGEOMETRY_H
#define MODELGEOMETRY_H
#include "IGeometry.h"
#include <vector>
#include <ContentManager/IContentManager.h>

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

    int GetMeshCount() {return meshes.size();}
    Material* GetMaterial(int mesh);

    int GetVerticesCount() {return verticesCount;}
    int GetIndicesCount() {return indicesCount;}

    Vertex3D* GetVertices() {return vertices;}
    U16* GetIndices() {return indices;}

    U16 GetMeshStartIndex(int mesh);
    U16 GetMeshIndicesCount(int mesh);

    void Draw(Camera* camera, const Matrix4x4& worldMatrix, ShaderProgram* shaderProgram);

private:
    VBO* vbo;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;

    Vertex3D* vertices;
    U16* indices;
    int verticesCount;
    int indicesCount;
};

#endif // MODELGEOMETRY_H
