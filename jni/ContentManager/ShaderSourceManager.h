#ifndef SHADERSOURCEMANAGER_H
#define SHADERSOURCEMANAGER_H
#include "IShaderSourceManager.h"

#include <map>
#include <string>


class ShaderSourceManager : public IShaderSourceManager
{
    ShaderSourceHandleManager handleManager;
    std::map<std::string, ShaderSourceHandle> loadedShaders;

public:
    ShaderSourceManager();
    ~ShaderSourceManager(){}

    ShaderSourceHandle GetShaderSource(const char *filename);
    void ReleaseShaderSource(ShaderSourceHandle &handle);
};

#endif // SHADERSOURCEMANAGER_H
