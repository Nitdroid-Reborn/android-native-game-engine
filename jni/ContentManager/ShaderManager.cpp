#include "ShaderManager.h"
#include <FileIO/IFileIO.h>

ShaderManager::ShaderManager() : IShaderManager() {
    ShaderHandle::manager = &handleManager;
    handleManager.SetDataFactory(new ShaderFactory());
    loadedShaders.clear();
}

ShaderManager::~ShaderManager() {
    loadedShaders.clear();
}

ShaderHandle ShaderManager::GetShader(const char *filename) {
    if(loadedShaders.count(filename)!=0)
        return loadedShaders[filename];

    ShaderHandle handle;
    Shader* shader = handleManager.Acquire(handle);

    IFileIO* io = IFileIO::get();

    unsigned int len = io->GetSize(filename);
    char* buffer = new char[len+1];

    io->Read(filename, buffer, len);

    buffer[len]='\0';

    std::string fname(filename);
    int p = fname.find(".vert");

    if(p!=std::string::npos) {
        shader->SetType(Shader::VertexShader);
    }
    else {
        shader->SetType(Shader::PixelShader);
    }

    shader->CompileSource(buffer, filename);

    delete[] buffer;

    loadedShaders[filename] = handle;
    return handle;
}


void ShaderManager::ReleaseShader(ShaderHandle &handle) {
    handleManager.Release(handle);
}
