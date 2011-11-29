#ifndef RENDERABLEGAMEOBJECT_H
#define RENDERABLEGAMEOBJECT_H
#include "GameObject.h"
#include "ContentManager/IContentManager.h"

class RenderableGameObject : public GameObject
{
public:
    RenderableGameObject(Hash _id, const Vector3& _position, const Vector3& _orientation,
                         const Vector3& _scale, std::string modelName, std::string shaderProgramName,
                         bool transparent=false);

    ~RenderableGameObject();

    void SetVisible(bool visible);
    bool GetVisible() const;

    std::string GetShaderProgram() const;
    void SetShaderProgram(std::string shaderProgram);

    std::string GetModel() const;
    void SetModel(std::string model);

    static GameObjectType GetType() {return RenderableGameObject::type;}
    void Update(float dt);

    void SetShaderProgramParameter(std::string name, const Vector2& value);
    void SetShaderProgramParameter(std::string name, const Vector3& value);
    void SetShaderProgramParameter(std::string name, const Vector4& value);
    void SetShaderProgramParameter(std::string name, const Matrix4x4& value);
    void SetShaderProgramParameter(std::string name, const F32 value);
    void SetShaderProgramParameter(std::string name, const I32 value);
    void ClearShaderParameters();

protected:
    static GameObjectType type;
    bool visible;
    bool transparent;
    std::string modelName;
    std::string shaderName;
    ModelGeometryHandle modelHandle;
    ShaderProgramHandle shaderProgramHandle;
    ShaderParametersList shaderParamsList;
};

#endif // RENDERABLEGAMEOBJECT_H
