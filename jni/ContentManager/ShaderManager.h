#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "IShaderManager.h"
#include <map>
#include <string>


class ShaderManager : public IShaderManager
{
    ShaderHandleManager handleManager;
    std::map<std::string, ShaderHandle> loadedShaders;

public:
    ShaderManager();
    ~ShaderManager();

    ShaderHandle GetShader(const char *filename);
    void ReleaseShader(ShaderHandle &handle);
};

#endif // SHADERMANAGER_H
