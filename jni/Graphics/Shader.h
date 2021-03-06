#ifndef SHADER_H
#define SHADER_H

#ifdef ANDROID
#include <GLES2/gl2.h>
#else
#include "GLee.h"
#endif

#include <string>

class Shader
{
public:
    enum ShaderType {
        VertexShader=0,
        PixelShader
    };

    Shader();
    ~Shader();
    void SetType(ShaderType type);
    bool CompileSource(const char* shaderSrc, const char* fileName);
    inline GLuint GetId() const {return id;}
    inline ShaderType GetType() const {return type;}
    inline bool IsCompiled() const {return compiled;}


private:
    ShaderType type;
    GLuint id;
    bool compiled;
};

#endif // SHADER_H
