#include "ShaderProgramManager.h"

ShaderProgramManager::ShaderProgramManager() : IShaderProgramManager() {
    ShaderProgramHandle::manager = &handleManager;
    handleManager.SetDataFactory(new ShaderProgramFactory());
    loadedPrograms.clear();
}

ShaderProgramManager::~ShaderProgramManager() {
    loadedPrograms.clear();
}

ShaderProgramHandle ShaderProgramManager::GetShaderProgram(const char *name) {
    if(loadedPrograms.count(name)!=0)
        return loadedPrograms[name];

    ShaderProgramHandle handle;
    handleManager.Acquire(handle);

    loadedPrograms[name] = handle;
    return handle;
}

void ShaderProgramManager::ReleaseShaderProgram(ShaderProgramHandle &handle) {
    handleManager.Release(handle);
}
