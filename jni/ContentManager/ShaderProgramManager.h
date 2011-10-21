#ifndef SHADERPROGRAMMANAGER_H
#define SHADERPROGRAMMANAGER_H
#include "IShaderProgramManager.h"
#include <map>
#include <string>

class ShaderProgramManager : public IShaderProgramManager
{
    ShaderProgramHandleManager handleManager;
    std::map<std::string, ShaderProgramHandle> loadedPrograms;
public:
    ShaderProgramManager();
    ~ShaderProgramManager();

    ShaderProgramHandle GetShaderProgram(const char *name);
    void ReleaseShaderProgram(ShaderProgramHandle &handle);
};

#endif // SHADERPROGRAMMANAGER_H
