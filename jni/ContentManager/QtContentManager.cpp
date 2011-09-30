#include "QtContentManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "ScriptSourceManager.h"

IContentManager* IContentManager::singleton=NULL;

QtContentManager::QtContentManager() : IContentManager()
{
}

QtContentManager::~QtContentManager() {

}


bool QtContentManager::Initialize() {
    ASSERT(!singleton, "ContentManager already initialized");
    singleton = this;


    textureManager = new TextureManager();
    soundManager = new SoundManager();
    scriptSourceManager = new ScriptSourceManager();


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

    Logger::Log(1, "Qt Content Manager initialized");
    return true;
}

bool QtContentManager::Release() {

    delete textureManager;
    delete soundManager;
    delete scriptSourceManager;

    textureManager = NULL;
    soundManager = NULL;
    scriptSourceManager = NULL;


    singleton = NULL;
    Logger::Log(1, "Qt Content Manager released");
    return true;
}
