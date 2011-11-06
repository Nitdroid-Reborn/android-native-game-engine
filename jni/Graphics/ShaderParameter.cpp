#include "ShaderParameter.h"
#include "ShaderProgram.h"

ShaderParameter::ShaderParameter(const ShaderParameter &other) {
    name = other.name;
    this->type = other.type;

    switch(this->type) {
        case ShaderParameter::FLOAT:
        {
            data = new F32;
            memcpy(data, other.data, sizeof(F32));
            break;
        }
        case ShaderParameter::INT:
        {
            data = new I32;
            memcpy(data, other.data, sizeof(I32));
            break;
        }
        case ShaderParameter::VECTOR2:
        {
            data = new Vector2;
            memcpy(data, other.data, sizeof(Vector2));
            break;
        }
        case ShaderParameter::VECTOR3:
        {
            data = new Vector3;
            memcpy(data, other.data, sizeof(Vector3));
            Vector3 v = GetDataAsVector3();
            break;
        }
        case ShaderParameter::VECTOR4:
        {
            data = new Vector4;
            memcpy(data, other.data, sizeof(Vector4));
            break;
        }
        case ShaderParameter::MATRIX:
        {
            data = new Matrix4x4();
            memcpy(data, other.data, sizeof(Matrix4x4));
            break;
        }
    }
}

ShaderParameter::ShaderParameter(std::string name, const Matrix4x4 &m)
{
    type = MATRIX;
    this->name = name;
    data = (void*)new Matrix4x4(m);
}

ShaderParameter::ShaderParameter(std::string name, const Vector2 &v) {
    this->name = name;
    type = VECTOR2;
    data = (void*)new Vector2(v);
}

ShaderParameter::ShaderParameter(std::string name, const Vector3 &v) {
    this->name = name;
    type = VECTOR3;
    data = (void*)new Vector3(v);
}

ShaderParameter::ShaderParameter(std::string name, const Vector4 &v) {
    this->name = name;
    type = VECTOR4;
    data = (void*)new Vector4(v);
}

ShaderParameter::ShaderParameter(std::string name, F32 value) {
    this->name = name;
    type = FLOAT;
    F32* temp = new F32;
    *temp = value;
    data = (void*)temp;
}

ShaderParameter::ShaderParameter(std::string name, I32 value) {
    this->name = name;
    type = INT;
    I32* temp = new I32;
    *temp = value;
    data = (void*)temp;
}

ShaderParameter::~ShaderParameter() {
    if(data)
        delete data;
}

Vector2 ShaderParameter::GetDataAsVector2() const{
    Vector2* pointer = static_cast<Vector2*>(data);
    return Vector2(*pointer);
}

Vector3 ShaderParameter::GetDataAsVector3() const{
    Vector3* pointer = static_cast<Vector3*>(data);
    return Vector3(*pointer);
}

Vector4 ShaderParameter::GetDataAsVector4() const{
    Vector4* pointer = static_cast<Vector4*>(data);
    return Vector4(*pointer);
}

Matrix4x4 ShaderParameter::GetDataAsMatrix4x4() const{
    Matrix4x4* pointer = static_cast<Matrix4x4*>(data);
    return Matrix4x4(*pointer);
}

F32 ShaderParameter::GetDataAsFloat() const{
    F32* pointer = static_cast<F32*>(data);
    return *pointer;
}

I32 ShaderParameter::GetDataAsInt() const{
    I32* pointer = static_cast<I32*>(data);
    return *pointer;
}



void ShaderParametersList::Add(std::string name, const Matrix4x4 &v) {
    parameters.push_back(ShaderParameter(name, v));
}

void ShaderParametersList::Add(std::string name, const Vector2 &v) {
    parameters.push_back(ShaderParameter(name, v));
}

void ShaderParametersList::Add(std::string name, const Vector3 &v) {
    parameters.push_back(ShaderParameter(name, v));
}

void ShaderParametersList::Add(std::string name, const Vector4 &v) {
    parameters.push_back(ShaderParameter(name, v));
}

void ShaderParametersList::Add(std::string name, F32 v) {
    parameters.push_back(ShaderParameter(name, v));
}

void ShaderParametersList::Add(std::string name, I32 v) {
    parameters.push_back(ShaderParameter(name, v));
}

void ShaderParametersList::Clear() {
    parameters.clear();
}

ShaderParametersList::ShaderParametersList(const ShaderParametersList &other) {
    parameters.clear();
    for(int i=0;i<other.GetSize();i++) {
        parameters.push_back(other.parameters[i]);
    }
}

ShaderParametersList& ShaderParametersList::operator =(const ShaderParametersList &other) {
    parameters.clear();
    for(int i=0;i<other.GetSize();i++) {
        parameters.push_back(other.parameters[i]);
    }
}

void ShaderParametersList::Apply(ShaderProgram *shaderProgram) const {

    for(int i=0; i<parameters.size(); i++) {
        switch(parameters[i].GetType()) {
        case ShaderParameter::FLOAT:
            shaderProgram->SetUniformValue(parameters[i].GetName(), parameters[i].GetDataAsFloat());
            break;
        case ShaderParameter::INT:
            shaderProgram->SetUniformValue(parameters[i].GetName(), parameters[i].GetDataAsInt());
            break;
        case ShaderParameter::VECTOR2:
            shaderProgram->SetUniformValue(parameters[i].GetName(), parameters[i].GetDataAsVector2());
            break;
        case ShaderParameter::VECTOR3:
        {
            shaderProgram->SetUniformValue(parameters[i].GetName(), parameters[i].GetDataAsVector3());
            break;
        }
        case ShaderParameter::VECTOR4:
            shaderProgram->SetUniformValue(parameters[i].GetName(), parameters[i].GetDataAsVector4());
            break;
        case ShaderParameter::MATRIX:
            shaderProgram->SetUniformValue(parameters[i].GetName(), parameters[i].GetDataAsMatrix4x4());
            break;
        }
    }

}
