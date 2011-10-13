#ifndef SHADER_H
#define SHADER_H
#include <string>
#ifdef ANDROID
#include <GLES2/gl2.h>
#else
#include "GLee.h"
#endif


class Shader
{
public:
    enum ShaderType {
        VertexShader=0,
        PixelShader
    };

    Shader(ShaderType type);
    ~Shader();
    bool CompileSource(std::string sourceCode);
    inline GLuint GetId() const {return id;}
    inline ShaderType GetType() const {return type;}
    inline bool IsCompiled() const {return compiled;}




private:
    ShaderType type;
    GLuint id;
    bool compiled;
};

#endif // SHADER_H
