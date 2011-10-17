#include "ShaderSourceManager.h"

ShaderSourceManager::ShaderSourceManager() : IShaderSourceManager() {
    ShaderSourceHandle::manager = &handleManager;
    handleManager.SetDataFactory(new ShaderSourceFactory());
}

ShaderSourceHandle ShaderSourceManager::GetShaderSource(const char *filename) {
    if(loadedShaders.count(filename)!=0)
        return loadedShaders[filename];

    ShaderSourceHandle handle;
    ShaderSource* shaderSrc = handleManager.Acquire(handle);
    shaderSrc->Load(filename);
    loadedShaders[filename] = handle;

    return handle;
}

void ShaderSourceManager::ReleaseShaderSource(ShaderSourceHandle &handle) {
    handleManager.Release(handle);
}
