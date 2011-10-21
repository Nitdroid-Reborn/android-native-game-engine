#ifndef ISHADERPROGRAMMANAGER_H
#define ISHADERPROGRAMMANAGER_H

#include "Handle.h"
#include "HandleManager.h"
#include <Graphics/ShaderProgram.h>

class ShaderProgramFactory {
public:
    ShaderProgram* Create() {return new ShaderProgram();}
};

struct tagShaderProgram{};
typedef Handle<ShaderProgram, ShaderProgramFactory, tagShaderProgram> ShaderProgramHandle;
typedef HandleManager<ShaderProgram, ShaderProgramFactory, tagShaderProgram> ShaderProgramHandleManager;



class IShaderProgramManager {
public:
    IShaderProgramManager(){}
    virtual ~IShaderProgramManager(){}

    virtual ShaderProgramHandle GetShaderProgram(const char* name)=0;

    virtual void ReleaseShaderProgram(ShaderProgramHandle& handle)=0;
};

#endif // ISHADERPROGRAMMANAGER_H
