#ifdef ANDROID
#ifndef ANDROIDCONTENTMANAGER_H
#define ANDROIDCONTENTMANAGER_H
#include "IContentManager.h"
#include <EGL/egl.h>
#include "TextureManager.h"

//! Android Content Manager implementation
class AndroidContentManager : public IContentManager
{
public:
    AndroidContentManager();
    ~AndroidContentManager();

    bool Initialize();
    bool Release();

    //! Returns EGLContext used by manager to load textures to OpenGL
    const EGLContext GetEGLContext() const;
private:
    EGLContext context;
    EGLDisplay display;
    EGLSurface surface;
};

#endif // ANDROIDCONTENTMANAGER_H
#endif
