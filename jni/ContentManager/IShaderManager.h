#ifndef ISHADERMANAGER_H
#define ISHADERMANAGER_H
#include "Handle.h"
#include "HandleManager.h"
#include <Graphics/Shader.h>

class ShaderFactory {
public:
    Shader* Create() {return new Shader();}
};

struct tagShader{};
typedef Handle<Shader, ShaderFactory, tagShader> ShaderHandle;
typedef HandleManager<Shader, ShaderFactory, tagShader> ShaderHandleManager;



class IShaderManager {
public:
    IShaderManager(){}
    virtual ~IShaderManager(){}

    virtual ShaderHandle GetShader(const char* filename)=0;

    virtual void ReleaseShader(ShaderHandle& handle)=0;
};

#endif // ISHADERMANAGER_H
