#ifdef ANDROID
#include "AndroidContentManager.h"
#include "SoundManager.h"
#include <Utils/Utils.h>
#include <Scripts/ScriptManager.h>
#include "ScriptSourceManager.h"
#include "ShaderSourceManager.h"

IContentManager* IContentManager::singleton=NULL;

AndroidContentManager::AndroidContentManager() : IContentManager()
{
    context = EGL_NO_CONTEXT;
}

AndroidContentManager::~AndroidContentManager() {

}

bool AndroidContentManager::Initialize() {
    ASSERT(!singleton, "ContentManager already initialized");
    singleton = this;


    textureManager = new TextureManager();
    soundManager = new SoundManager();
    scriptSourceManager = new ScriptSourceManager();
    shaderSourceManager = new ShaderSourceManager();


    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
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

    int attrib_list[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    context = eglCreateContext(display, config, NULL, attrib_list);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        Logger::Log(3, "Unable to eglMakeCurrent in context manager %d", eglGetError());
        return false;
    }



    //Register to lua
    ScriptManager* manager = ScriptManager::Get();
    lua_State* L = manager->getState();
    luabind::module(L)
    [
        luabind::class_<IContentManager>("ContentManager")
            .def("GetSoundManager", &IContentManager::GetSoundManager)
            .def("GetTextureManager", &IContentManager::GetTextureManager)
            .scope
            [
                luabind::def("Get", &IContentManager::get)
            ]
    ];

    luabind::module(L)
    [
        luabind::class_<ISoundManager>("SoundManager")
            .def("GetSound", &ISoundManager::GetSound)
            .def("ReleaseSound", &ISoundManager::ReleaseSound)
    ];



    luabind::module(L)
    [
        luabind::class_<SoundHandle>("SoundHandle")
            .def("Get", (ISound* (SoundHandle::*) (void))&SoundHandle::Get)
    ];

    luabind::module(L)
    [
        luabind::class_<ITextureManager>("TextureManager")
            .def("GetTexture", (TextureHandle (ITextureManager::*) (const char*))&ITextureManager::GetTexture)
            .def("ReleaseTexture", &ITextureManager::ReleaseTexture)
    ];

    luabind::module(L)
    [
        luabind::class_<TextureHandle>("TextureHandle")
            .def("Get", (ITexture* (TextureHandle::*) (void))&TextureHandle::Get)
    ];






    Logger::Log(1, "Android Content Manager initialized");
    return true;
}

bool AndroidContentManager::Release() {

    delete textureManager;
    delete soundManager;
    delete scriptSourceManager;
    delete shaderSourceManager;

    textureManager = NULL;
    soundManager = NULL;
    scriptSourceManager = NULL;


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

    singleton = NULL;
    Logger::Log(1, "Android Content Manager released");
    return true;
}

const EGLContext AndroidContentManager::GetEGLContext() const{
    return context;
}

#endif
