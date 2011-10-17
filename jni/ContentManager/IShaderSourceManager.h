#ifndef ISHADERSOURCEMANAGER_H
#define ISHADERSOURCEMANAGER_H
#include "Handle.h"
#include "HandleManager.h"

#include <Graphics/ShaderSource.h>
#include <Graphics/ShaderSourceFactory.h>

struct tagShader{};
typedef Handle<ShaderSource, ShaderSourceFactory, tagShader> ShaderSourceHandle;
typedef HandleManager<ShaderSource, ShaderSourceFactory, tagShader> ShaderSourceHandleManager;


//! Interface for shader source manager
class IShaderSourceManager {
public:
    IShaderSourceManager(){}
    virtual ~IShaderSourceManager(){}

    //! Gets shader source
    /*!
      If source is not loaded, loads it to memory, otherwise returns handle to it.
      \param filename path to shader file
      \returns handle to shader source
    */
    virtual ShaderSourceHandle GetShaderSource(const char* filename)=0;

    //! Releases shader source
    /*!
      Releases reference to shader source from handle, if reference count for this source drops to 0 deletes it
      \param handle handle to shader source
      */
    virtual void ReleaseShaderSource(ShaderSourceHandle& handle)=0;
};
#endif // ISHADERSOURCEMANAGER_H
