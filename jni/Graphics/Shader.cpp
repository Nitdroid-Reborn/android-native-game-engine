#include "Shader.h"

Shader::Shader(ShaderType type)
{
    this->type = type;
    compiled = false;

    if(type == VertexShader)
        id = glCreateShader(GL_VERTEX_SHADER);
    else
        id = glCreateShader(GL_PIXEL_SHADER);

}

Shader::~Shader() {
    if(id) {
        glDeleteShader(id);
        id=0;
    }
}

GLuint Shader::GetId() const {
    return id;
}

bool Shader::IsCompiled() const {
    return compiled;
}

ShaderType Shader::GetType() const {
    return type;
}
bool Shader::CompileSource(std::string sourceCode) {
    glShaderSource(shader, 1, &sourceCode.c_str(), NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* buf = (char*) malloc(infoLen);
            if (buf) {
                glGetShaderInfoLog(shader, infoLen, NULL, buf);
                LOGE("Could not compile shader %d:\n%s\n",
                        shaderType, buf);
                free(buf);
            }
            glDeleteShader(shader);
            shader = 0;
        }
        return false;
    }

    return true;
}



