#ifndef SHADER_H
#define SHADER_H
#include <string>
#ifdef ANDROID
#include <GLES2/gl2.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>

#endif

enum ShaderType {
    VertexShader=0,
    PixelShader
};

class Shader
{
public:
    Shader(ShaderType type);
    ~Shader();
    bool CompileSource(std::string sourceCode);
    GLuint GetId() const;
    ShaderType GetType() const;
    bool IsCompiled() const;



private:
    ShaderType type;
    GLuint id;
    bool compiled;
};

#endif // SHADER_H
