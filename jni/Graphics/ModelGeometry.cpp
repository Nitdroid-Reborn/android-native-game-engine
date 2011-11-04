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
    materials = NULL;
    meshes = NULL;
    meshesCount = 0;
    materialsCount = 0;
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

    if(meshes) {
        delete[] meshes;
        meshes = 0;
    }

    if(materials) {
        delete[] materials;
        materials = 0;
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
               meshes, meshesCount,
               materials, materialsCount);

    Logger::Log("Loaded %d %d", verticesCount, indicesCount);
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
    shaderProgram->EnableAttributeArray("vNormal");

    Matrix4x4 mv = camera->GetViewMatrix()*worldMatrix;
    Matrix4x4 mvp = camera->GetProjectionMatrix()*mv;

    shaderProgram->SetUniformValue("modelViewMatrix", mv);
    shaderProgram->SetUniformValue("modelViewProjectionMatrix", mvp);
    shaderProgram->SetUniformValue("textureSampler", 0);



    vbo->Bind();

/*    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(mvp.entries);*/



    shaderProgram->SetAttributeArray(vbo);
    //materials[meshes[0].materialIndex].texture.Get()->Bind();

    //vbo->Draw(0, indicesCount/3);
    for(int i=0;i<meshesCount;i++) {
        shaderProgram->SetUniformValue("materialDiffuse", materials[meshes[i].materialIndex].diffuse[0],
                                                          materials[meshes[i].materialIndex].diffuse[1],
                                                          materials[meshes[i].materialIndex].diffuse[2]);
        shaderProgram->SetUniformValue("materialAmbient", materials[meshes[i].materialIndex].ambient[0],
                                                          materials[meshes[i].materialIndex].ambient[1],
                                                          materials[meshes[i].materialIndex].ambient[2]);
        shaderProgram->SetUniformValue("materialSpecular",materials[meshes[i].materialIndex].specular[0],
                                                          materials[meshes[i].materialIndex].specular[1],
                                                          materials[meshes[i].materialIndex].specular[2]);

        shaderProgram->SetUniformValue("materialShininess", materials[meshes[i].materialIndex].shinniness);
        if(materials[meshes[i].materialIndex].texture.Get())
            materials[meshes[i].materialIndex].texture.Get()->Bind();

        vbo->Draw(meshes[i].startIndex, meshes[i].indicesCount/3);
    }


    vbo->Release();
    shaderProgram->Release();

}
