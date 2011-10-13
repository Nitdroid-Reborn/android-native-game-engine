#include "Shader.h"
#include <Utils/Log.h>

Shader::Shader(ShaderType type)
{
    this->type = type;
    compiled = false;

    if(type == VertexShader)
        id = glCreateShader(GL_VERTEX_SHADER);
    else
        id = glCreateShader(GL_FRAGMENT_SHADER);

}

Shader::~Shader() {
    if(id) {
        glDeleteShader(id);
        id=0;
    }
}

bool Shader::CompileSource(std::string sourceCode) {
    const char* src = sourceCode.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    GLint compiled = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* buf = new char[infoLen];
            if (buf) {
                glGetShaderInfoLog(id, infoLen, NULL, buf);
                Logger::Log("Could not compile shader:\n%s\n", buf);
                delete[] buf;
            }
            glDeleteShader(id);
            id = 0;
        }
        return false;
    }

    return true;
}



