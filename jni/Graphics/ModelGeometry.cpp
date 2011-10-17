#include "ModelGeometry.h"
#include "MS3DModel.h"
#include <Utils/Log.h>
#include "VBO.h"
#include "ShaderProgram.h"
#include "Camera.h"

ModelGeometry::ModelGeometry()
{
    indices=NULL;
    indicesCount=0;
    vertices=NULL;
    verticesCount=0;
    vbo = NULL;
}

ModelGeometry::~ModelGeometry() {
    if(verticesCount) {
        delete[] vertices;
        vertices = 0;
    }
    if(indicesCount) {
        delete[] indices;
        indices = 0;
    }

    if(vbo) {
        delete vbo;
        vbo = 0;
    }
}


void ModelGeometry::Load(const char *filename) {
    MS3DModel model;
    model.Load(filename, vertices, verticesCount,
               indices, indicesCount,
               meshes, materials);

    if(vbo)
        delete vbo;

    vbo = new VBO(this);
}


Material* ModelGeometry::GetMaterial(int mesh) {
    return &materials[meshes[mesh].materialIndex];
}

U16 ModelGeometry::GetMeshIndicesCount(int mesh) {
    return meshes[mesh].indicesCount;
}

U16 ModelGeometry::GetMeshStartIndex(int mesh) {
    return meshes[mesh].startIndex;
}


void ModelGeometry::Draw(Camera* camera, const Matrix4x4& worldMatrix, ShaderProgram *shaderProgram) {
    shaderProgram->Bind();
    shaderProgram->EnableAttributeArray("vPosition");
    shaderProgram->EnableAttributeArray("vTexCoords");
    shaderProgram->EnableAttributeArray("vColor");

    Matrix4x4 mvp = camera->GetProjectionMatrix()*camera->GetViewMatrix()*worldMatrix;

    shaderProgram->SetUniformValue("mvp", mvp);
    shaderProgram->SetUniformValue("textureSampler", 0);

    vbo->Bind();

    shaderProgram->SetAttributeArray(vbo);
    //materials[meshes[0].materialIndex].texture.Get()->Bind();

    //vbo->Draw(0, indicesCount/3);
    for(int i=0;i<meshes.size();i++) {
        materials[meshes[i].materialIndex].texture.Get()->Bind();
        vbo->Draw(meshes[i].startIndex, meshes[i].indicesCount/3);
    }

    vbo->Release();
    shaderProgram->Release();

}
