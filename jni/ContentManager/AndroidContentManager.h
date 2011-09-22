#ifndef ANDROIDCONTENTMANAGER_H
#define ANDROIDCONTENTMANAGER_H
#include "IContentManager.h"
#include <EGL/egl.h>
#include "TextureManager.h"

class AndroidContentManager : public IContentManager
{
public:
    AndroidContentManager();
    ~AndroidContentManager();

    bool Initialize();
    bool Release();

    void OnInitWindow();

    const EGLContext GetEGLContext() const;

private:

    EGLContext context;
    EGLDisplay display;
    EGLSurface surface;
};


/*OOLUA_PROXY_CLASS(AndroidContentManager, IContentManager)
    OOLUA_NO_TYPEDEFS
    OOLUA_ONLY_DEFAULT_CONSTRUCTOR
OOLUA_CLASS_END*/

#endif // ANDROIDCONTENTMANAGER_H
