#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <string>
#include <vector>
#ifdef ANDROID
#include <GLES2/gl2.h>
#else
#include "GLee.h"
#endif
#include <Utils/EngineTypes.h>
#include <Math/MathLib.h>
#include "VBO.h"
#include <ContentManager/ShaderManager.h>

class Shader;

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();
    void AddShader(Shader* s);
    void AddShader(ShaderHandle s);
    void RemoveShader(Shader* s);

    void Bind();
    void Release();
    int GetAttributeLocation(std::string name);
    int GetUniformLocation(std::string name);

    void EnableAttributeArray(int attr);
    void EnableAttributeArray(std::string name);

    void DisableAttributeArray(int attr);
    void DisableAttributeArray(std::string name);

    inline bool IsLinked() const {return linked;}
    bool Link();
    inline GLuint GetId() const {return id;}


    void SetAttributeArray(VBO* vbo);

    /*void SetAttributeArray(int attr, int size, GLenum type, bool normalized,
                           GLsizei stride, const GLvoid * pointer);
    void SetAttributeArray(std::string name, int size, GLenum type, bool normalized,
                           GLsizei stride, const GLvoid * pointer);
*/

    void SetAttributeValue(int attr, F32 x);
    void SetAttributeValue(int attr, F32 x, F32 y);
    void SetAttributeValue(int attr, F32 x, F32 y, F32 z);
    void SetAttributeValue(int attr, F32 x, F32 y, F32 z, F32 w);
    void SetAttributeValue(int attr, const Vector2&);
    void SetAttributeValue(int attr, const Vector3&);
    void SetAttributeValue(int attr, const Vector4&);

    void SetAttributeValue(std::string name, F32 x);
    void SetAttributeValue(std::string name, F32 x, F32 y);
    void SetAttributeValue(std::string name, F32 x, F32 y, F32 z);
    void SetAttributeValue(std::string name, F32 x, F32 y, F32 z, F32 w);
    void SetAttributeValue(std::string name, const Vector2&);
    void SetAttributeValue(std::string name, const Vector3&);
    void SetAttributeValue(std::string name, const Vector4&);


    void SetUniformValue(int attr, F32 x);
    void SetUniformValue(int attr, F32 x, F32 y);
    void SetUniformValue(int attr, F32 x, F32 y, F32 z);
    void SetUniformValue(int attr, F32 x, F32 y, F32 z, F32 w);
    void SetUniformValue(int attr, const Vector2&);
    void SetUniformValue(int attr, const Vector3&);
    void SetUniformValue(int attr, const Vector4&);
    void SetUniformValue(int attr, const Matrix4x4&);
    void SetUniformValue(int attr, I32 x);
    void SetUniformValue(int attr, I32 x, I32 y);
    void SetUniformValue(int attr, I32 x, I32 y, I32 z);
    void SetUniformValue(int attr, I32 x, I32 y, I32 z, I32 w);

    void SetUniformValue(std::string name, F32 x);
    void SetUniformValue(std::string name, F32 x, F32 y);
    void SetUniformValue(std::string name, F32 x, F32 y, F32 z);
    void SetUniformValue(std::string name, F32 x, F32 y, F32 z, F32 w);
    void SetUniformValue(std::string name, const Vector2&);
    void SetUniformValue(std::string name, const Vector3&);
    void SetUniformValue(std::string name, const Vector4&);
    void SetUniformValue(std::string name, const Matrix4x4&);
    void SetUniformValue(std::string name, I32 x);
    void SetUniformValue(std::string name, I32 x, I32 y);
    void SetUniformValue(std::string name, I32 x, I32 y, I32 z);
    void SetUniformValue(std::string name, I32 x, I32 y, I32 z, I32 w);


private:
    GLuint id;
    bool linked;
    std::vector<ShaderHandle> shaders;
};

#endif // SHADERPROGRAM_H
