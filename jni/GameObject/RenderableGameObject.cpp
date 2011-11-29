#include "RenderableGameObject.h"
#include "Graphics/IRenderer.h"

GameObjectType RenderableGameObject::type = Renderable;

RenderableGameObject::RenderableGameObject(Hash _id, const Vector3 &_position, const Vector3 &_orientation, const Vector3 &_scale,
                                           std::string modelName, std::string shaderProgramName, bool transparent) : GameObject(_id, _position, _orientation, _scale)
{

    IContentManager* manager = IContentManager::get();
    this->modelName = modelName;
    this->shaderName = shaderProgramName;
    modelHandle = manager->GetModelGeometryManager()->GetModelGeometry(modelName.c_str());
    shaderProgramHandle = manager->GetShaderProgramManager()->GetShaderProgram(shaderProgramName.c_str());
    this->transparent = transparent;
    visible = true;
}

RenderableGameObject::~RenderableGameObject() {
    IContentManager::get()->GetModelGeometryManager()->ReleaseModelGeometry(modelHandle);
    IContentManager::get()->GetShaderProgramManager()->ReleaseShaderProgram(shaderProgramHandle);
}

void RenderableGameObject::SetVisible(bool visible) {
    this->visible = visible;
}

bool RenderableGameObject::GetVisible() const {
    return visible;
}

std::string RenderableGameObject::GetShaderProgram() const {
    return shaderName;
}

std::string RenderableGameObject::GetModel() const {
    return modelName;
}

void RenderableGameObject::SetModel(std::string model) {
    IContentManager::get()->GetModelGeometryManager()->ReleaseModelGeometry(modelHandle);
    modelHandle = IContentManager::get()->GetModelGeometryManager()->GetModelGeometry(model.c_str());
    modelName = model;
}

void RenderableGameObject::SetShaderProgram(string shaderProgram) {
    IContentManager::get()->GetShaderProgramManager()->ReleaseShaderProgram(shaderProgramHandle);
    shaderProgramHandle = IContentManager::get()->GetShaderProgramManager()->GetShaderProgram(shaderProgram.c_str());
    shaderName = shaderProgram;
}

void RenderableGameObject::SetShaderProgramParameter(string name, const F32 value) {
    shaderParamsList.Add(name, value);
}

void RenderableGameObject::SetShaderProgramParameter(string name, const I32 value) {
    shaderParamsList.Add(name, value);
}

void RenderableGameObject::SetShaderProgramParameter(string name, const Vector2& value) {
    shaderParamsList.Add(name, value);
}

void RenderableGameObject::SetShaderProgramParameter(string name, const Vector3& value) {
    shaderParamsList.Add(name, value);
}

void RenderableGameObject::SetShaderProgramParameter(string name, const Vector4& value) {
    shaderParamsList.Add(name, value);
}
void RenderableGameObject::SetShaderProgramParameter(string name, const Matrix4x4& value) {
    shaderParamsList.Add(name, value);
}

void RenderableGameObject::ClearShaderParameters() {
    shaderParamsList.Clear();
}


void RenderableGameObject::Update(float dt) {
    if(visible) {
        Matrix4x4 translation;
        Matrix4x4 rotation;
        Matrix4x4 scale;

        translation.SetTranslation(position);
        rotation.SetRotationEuler(orientation.x, orientation.y, orientation.z);
        scale.SetScale(this->scale);

        Matrix4x4 worldMatrix = translation*rotation*scale;

        IRenderer::get()->DrawGeometry(modelHandle, worldMatrix, shaderProgramHandle, &shaderParamsList, transparent);
    }
}
