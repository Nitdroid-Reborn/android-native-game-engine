#ifndef SHADERPARAMETER_H
#define SHADERPARAMETER_H
#include <Math/MathLib.h>
#include <Utils/EngineTypes.h>

class ShaderProgram;

class ShaderParameter
{
public:
    ShaderParameter(){data=0;}
    ShaderParameter(std::string name, const Vector2&);
    ShaderParameter(std::string name, const Vector3&);
    ShaderParameter(std::string name, const Vector4&);
    ShaderParameter(std::string name, const Matrix4x4&);
    ShaderParameter(std::string name, F32);
    ShaderParameter(std::string name, I32);
    ShaderParameter(const ShaderParameter& other);
    ~ShaderParameter();

    enum ShaderParameterType {
        FLOAT=0,
        VECTOR2,
        VECTOR3,
        VECTOR4,
        MATRIX,
        INT
    };

    std::string GetName() const {return name;}
    ShaderParameterType GetType() const {return type;}
    Vector2 GetDataAsVector2() const;
    Vector3 GetDataAsVector3() const;
    Vector4 GetDataAsVector4() const;
    Matrix4x4 GetDataAsMatrix4x4() const;
    F32 GetDataAsFloat() const;
    I32 GetDataAsInt() const;

private:
    std::string name;
    void* data;
    ShaderParameterType type;
};

class ShaderParametersList {
public:
    ShaderParametersList(){}
    ShaderParametersList(const ShaderParametersList& other);
    ShaderParametersList& operator=(const ShaderParametersList& );

    void Add(std::string name, const Vector2&);
    void Add(std::string name, const Vector3&);
    void Add(std::string name, const Vector4&);
    void Add(std::string name, const Matrix4x4&);
    void Add(std::string name, F32);
    void Add(std::string name, I32);


    void Apply(ShaderProgram* shader) const;
    U32 GetSize() const {return parameters.size();}
    void Clear();
private:

    std::vector<ShaderParameter> parameters;
};


#endif // SHADERPARAMETER_H
