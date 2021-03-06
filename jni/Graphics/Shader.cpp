#include "Shader.h"
#include <Utils/Log.h>
#include <Utils/Assert.h>

Shader::Shader()
{
    compiled = false;
    id=0;
}

void Shader::SetType(ShaderType type) {
    ASSERT(id==0, "Shader already intialized");

   this->type = type;
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

static const char minimalVertexShader[] =
    "attribute mediump vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = (vPosition);\n"
    "}\n";

static const char minimalFragmentShader[] =
    "void main() {\n"
    "  gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
    "}\n";

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        Logger::Log(LOG_ERROR, 0, "after %s() glError (0x%x)\n", op, error);
    }
}

bool Shader::CompileSource(const char* src, const char* fileName) {

    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    GLint compiled = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if (!compiled || src[0]=='\0') {
        GLint infoLen = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* buf = new char[infoLen];
            if (buf) {
                glGetShaderInfoLog(id, infoLen, NULL, buf);
                Logger::Log("Could not compile shader %s: %s\n", fileName, buf);
                Logger::Log("%s", src);
                delete[] buf;
            }   
        }
        if(type == VertexShader) {
            std::string src(minimalVertexShader);
            const char* s = src.c_str();
            glShaderSource(id, 1, &s, NULL);
        }
        else {
            std::string src(minimalFragmentShader);
            const char* s = src.c_str();
            glShaderSource(id, 1, &s, NULL);
        }
        glCompileShader(id);

        return false;
    }

    return true;
}



