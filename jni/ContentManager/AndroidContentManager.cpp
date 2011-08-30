#include "AndroidContentManager.h"
#include "../Graphics/Texture.h"
#include <Utils/Utils.h>

IContentManager* IContentManager::singleton=NULL;

AndroidContentManager::AndroidContentManager() : IContentManager()
{
    context = EGL_NO_CONTEXT;
}

AndroidContentManager::~AndroidContentManager() {

}

bool AndroidContentManager::Initialize() {
    if(singleton != NULL) {
        assert(!singleton);
    }
    singleton = this;


    LOGI("Initializing android content manager");
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };

    EGLint numConfigs;
    EGLConfig config;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    const EGLint surfaceAttribs[] = {
        EGL_WIDTH, 16,
        EGL_HEIGHT, 16,
        EGL_NONE
    };

    surface = eglCreatePbufferSurface(display, config, surfaceAttribs);

    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGE("Unable to eglMakeCurrent in context manager %d", eglGetError());
        return false;
    }

    return true;
}

bool AndroidContentManager::Release() {
    if(display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if(context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if(surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;

    return true;
}

const EGLContext AndroidContentManager::GetEGLContext() const{
    return context;
}

void AndroidContentManager::LoadTexture(char *filename) {
    /*tex = Texture(filename);
    tex.Load();
    LOGI("Texture id: %d", tex.textureId);*/
}
