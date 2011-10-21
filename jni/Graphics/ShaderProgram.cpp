#include "ShaderProgram.h"
#include "Shader.h"
#include <Utils/Log.h>
#include <ContentManager/IContentManager.h>

ShaderProgram::ShaderProgram()
{
    id = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    if(id) {
        for(int i=0;i<shaders.size();i++)
            IContentManager::get()->GetShaderManager()->ReleaseShader(shaders[i]);

        glDeleteProgram(id);
        id=0;
    }
}

void ShaderProgram::AddShader(Shader *s) {
    glAttachShader(id, s->GetId());
}

void ShaderProgram::AddShader(ShaderHandle s) {
    glAttachShader(id, s.Get()->GetId());
    shaders.push_back(s);
}

void ShaderProgram::RemoveShader(Shader *s) {
    glDetachShader(id, s->GetId());
}

int ShaderProgram::GetAttributeLocation(std::string name) {
    return (int)glGetAttribLocation(id, name.c_str());
}

int ShaderProgram::GetUniformLocation(std::string name) {
    return (int)glGetUniformLocation(id, name.c_str());
}


bool ShaderProgram::Link() {
    if(id) {
        glLinkProgram(id);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = new char[bufLength];
                if (buf) {
                    glGetProgramInfoLog(id, bufLength, NULL, buf);
                    Logger::Log("Could not link program:\n%s\n", buf);
                    delete[] buf;
                }
            }
            glDeleteProgram(id);
            id = 0;
            linked = false;
            return false;
        }
        linked = true;
        return true;
    }
    linked = false;
    return false;
}



void ShaderProgram::Bind() {
    glUseProgram(id);
}

void ShaderProgram::Release() {
    glUseProgram(0);
}

void ShaderProgram::EnableAttributeArray(int attr) {
    glEnableVertexAttribArray(attr);
}

void ShaderProgram::EnableAttributeArray(std::string name) {
    int attr = GetAttributeLocation(name);
    EnableAttributeArray(attr);
}

void ShaderProgram::DisableAttributeArray(int attr) {
    glDisableVertexAttribArray(attr);
}

void ShaderProgram::DisableAttributeArray(std::string name) {
    int attr = GetAttributeLocation(name);
    glDisableVertexAttribArray(attr);
}

void ShaderProgram::SetAttributeValue(int attr, F32 x) {
    glVertexAttrib1f(attr, x);
}

void ShaderProgram::SetAttributeValue(int attr, F32 x, F32 y) {
    glVertexAttrib2f(attr, x, y);
}

void ShaderProgram::SetAttributeValue(int attr, F32 x, F32 y, F32 z) {
    glVertexAttrib3f(attr, x, y, z);
}

void ShaderProgram::SetAttributeValue(int attr, F32 x, F32 y, F32 z, F32 w) {
    glVertexAttrib4f(attr, x, y, z, w);
}

void ShaderProgram::SetAttributeValue(int attr, const Vector2& v) {
    glVertexAttrib2f(attr, v.x, v.y);
}

void ShaderProgram::SetAttributeValue(int attr, const Vector3& v) {
    glVertexAttrib3f(attr, v.x, v.y, v.z);
}

void ShaderProgram::SetAttributeValue(int attr, const Vector4& v) {
    glVertexAttrib4f(attr, v.x, v.y, v.z, v.w);
}

void ShaderProgram::SetAttributeValue(std::string name, F32 x) {
    glVertexAttrib1f(GetAttributeLocation(name), x);
}

void ShaderProgram::SetAttributeValue(std::string name, F32 x, F32 y) {
    glVertexAttrib2f(GetAttributeLocation(name), x, y);
}

void ShaderProgram::SetAttributeValue(std::string name, F32 x, F32 y, F32 z) {
    glVertexAttrib3f(GetAttributeLocation(name), x, y, z);
}

void ShaderProgram::SetAttributeValue(std::string name, F32 x, F32 y, F32 z, F32 w) {
    glVertexAttrib4f(GetAttributeLocation(name), x, y, z, w);
}

void ShaderProgram::SetAttributeValue(std::string name, const Vector2& v) {
    glVertexAttrib2f(GetAttributeLocation(name), v.x, v.y);
}

void ShaderProgram::SetAttributeValue(std::string name, const Vector3& v) {
    glVertexAttrib3f(GetAttributeLocation(name), v.x, v.y, v.z);
}

void ShaderProgram::SetAttributeValue(std::string name, const Vector4& v) {
    glVertexAttrib4f(GetAttributeLocation(name), v.x, v.y, v.z, v.w);
}


void ShaderProgram::SetUniformValue(int attr, F32 x) {
    glUniform1f(attr, x);
}

void ShaderProgram::SetUniformValue(int attr, F32 x, F32 y) {
    glUniform2f(attr, x, y);
}

void ShaderProgram::SetUniformValue(int attr, F32 x, F32 y, F32 z) {
    glUniform3f(attr, x, y, z);
}

void ShaderProgram::SetUniformValue(int attr, F32 x, F32 y, F32 z, F32 w) {
    glUniform4f(attr, x, y, z, w);
}

void ShaderProgram::SetUniformValue(int attr, I32 x) {
    glUniform1i(attr, x);
}

void ShaderProgram::SetUniformValue(int attr, I32 x, I32 y) {
    glUniform2i(attr, x, y);
}

void ShaderProgram::SetUniformValue(int attr, I32 x, I32 y, I32 z) {
    glUniform3i(attr, x, y, z);
}

void ShaderProgram::SetUniformValue(int attr, I32 x, I32 y, I32 z, I32 w) {
    glUniform4i(attr, x, y, z, w);
}

void ShaderProgram::SetUniformValue(int attr, const Vector2 & v) {
    glUniform2f(attr, v.x, v.y);
}

void ShaderProgram::SetUniformValue(int attr, const Vector3 & v) {
    glUniform3f(attr, v.x, v.y, v.z);
}

void ShaderProgram::SetUniformValue(int attr, const Vector4 & v) {
    glUniform4f(attr, v.x, v.y, v.z, v.w);
}

void ShaderProgram::SetUniformValue(int attr, const Matrix4x4 &m) {
    glUniformMatrix4fv(attr, 1, GL_FALSE, &m.entries[0]);
}


void ShaderProgram::SetUniformValue(std::string name, F32 x) {
    glUniform1f(GetUniformLocation(name), x);
}

void ShaderProgram::SetUniformValue(std::string name, F32 x, F32 y) {
    glUniform2f(GetUniformLocation(name), x, y);
}

void ShaderProgram::SetUniformValue(std::string name, F32 x, F32 y, F32 z) {
    glUniform3f(GetUniformLocation(name), x, y, z);
}

void ShaderProgram::SetUniformValue(std::string name, F32 x, F32 y, F32 z, F32 w) {
    glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void ShaderProgram::SetUniformValue(std::string name, I32 x) {
    glUniform1i(GetUniformLocation(name), x);
}

void ShaderProgram::SetUniformValue(std::string name, I32 x, I32 y) {
    glUniform2i(GetUniformLocation(name), x, y);
}

void ShaderProgram::SetUniformValue(std::string name, I32 x, I32 y, I32 z) {
    glUniform3i(GetUniformLocation(name), x, y, z);
}

void ShaderProgram::SetUniformValue(std::string name, I32 x, I32 y, I32 z, I32 w) {
    glUniform4i(GetUniformLocation(name), x, y, z, w);
}

void ShaderProgram::SetUniformValue(std::string name, const Vector2 & v) {
    glUniform2f(GetUniformLocation(name), v.x, v.y);
}

void ShaderProgram::SetUniformValue(std::string name, const Vector3 & v) {
    glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
}

void ShaderProgram::SetUniformValue(std::string name, const Vector4 & v) {
    glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w);
}

void ShaderProgram::SetUniformValue(std::string name, const Matrix4x4 &m) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &m.entries[0]);
}



void ShaderProgram::SetAttributeArray(VBO *vbo) {
    glVertexAttribPointer(GetAttributeLocation("vPosition"),
                          vbo->GetVertexAttributeSize(VertexPosition),
                          vbo->GetVertexAttributeType(VertexPosition),
                          vbo->GetVertexAttributeNormalization(VertexPosition),
                          vbo->GetVertexAttributeStride(VertexPosition),
                          vbo->GetVertexAttributeOffset(VertexPosition));

    glVertexAttribPointer(GetAttributeLocation("vNormal"),
                          vbo->GetVertexAttributeSize(VertexNormal),
                          vbo->GetVertexAttributeType(VertexNormal),
                          vbo->GetVertexAttributeNormalization(VertexNormal),
                          vbo->GetVertexAttributeStride(VertexNormal),
                          vbo->GetVertexAttributeOffset(VertexNormal));

    glVertexAttribPointer(GetAttributeLocation("vTexCoords"),
                          vbo->GetVertexAttributeSize(VertexTexCoord),
                          vbo->GetVertexAttributeType(VertexTexCoord),
                          vbo->GetVertexAttributeNormalization(VertexTexCoord),
                          vbo->GetVertexAttributeStride(VertexTexCoord),
                          vbo->GetVertexAttributeOffset(VertexTexCoord));

    glVertexAttribPointer(GetAttributeLocation("vColor"),
                          vbo->GetVertexAttributeSize(VertexColor),
                          vbo->GetVertexAttributeType(VertexColor),
                          vbo->GetVertexAttributeNormalization(VertexColor),
                          vbo->GetVertexAttributeStride(VertexColor),
                          vbo->GetVertexAttributeOffset(VertexColor));
}

/*
void ShaderProgram::SetAttributeArray(int attr, int size, GLenum type, bool normalized, GLsizei stride, const GLvoid *pointer) {
    glVertexAttribPointer(attr, size, type, normalized, stride, pointer);
}

void ShaderProgram::SetAttributeArray(std::string name, int size, GLenum type, bool normalized, GLsizei stride, const GLvoid *pointer) {
    glVertexAttribPointer(GetAttributeLocation(name), size, type, normalized, stride, pointer);
}*/
