#ifndef ANDROIDCONTENTMANAGER_H
#define ANDROIDCONTENTMANAGER_H
#include "IContentManager.h"
#include <EGL/egl.h>


class AndroidContentManager : public IContentManager
{
public:
    AndroidContentManager();
    ~AndroidContentManager();

    bool Initialize();
    bool Release();

    void OnInitWindow();
    void LoadTexture(char* filename);

    const EGLContext GetEGLContext() const;

private:
    EGLContext context;
    EGLDisplay display;
    EGLSurface surface;
};

#endif // ANDROIDCONTENTMANAGER_H
