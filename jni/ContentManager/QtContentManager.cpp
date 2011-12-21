#include "QtContentManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "ScriptSourceManager.h"
#include "ShaderManager.h"
#include <Scripts/ScriptManager.h>
#include "ShaderProgramManager.h"
#include "ModelGeometryManager.h"

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
    shaderManager = new ShaderManager();
    shaderProgramManager = new ShaderProgramManager();
    modelGeometryManager = new ModelGeometryManager();


    //Register to lua
    ScriptManager* manager = ScriptManager::Get();
    lua_State* L = manager->getState();
    luabind::module(L)
    [
        luabind::class_<IContentManager>("ContentManager")
            .def("GetSoundManager", &IContentManager::GetSoundManager)
            .def("GetTextureManager", &IContentManager::GetTextureManager)
            .def("GetShaderManager", &IContentManager::GetShaderManager)
            .def("GetShaderProgramManager", &IContentManager::GetShaderProgramManager)
            .def("GetModelGeometryManager", &IContentManager::GetModelGeometryManager)
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


    luabind::module(L)
    [
        luabind::class_<IShaderManager>("ShaderManager")
            .def("GetShader", &IShaderManager::GetShader)
            .def("ReleaseShader", &IShaderManager::ReleaseShader)
    ];

    luabind::module(L)
    [
        luabind::class_<ShaderHandle>("ShaderHandle")
    ];


    luabind::module(L)
    [
        luabind::class_<IShaderProgramManager>("ShaderProgramManager")
            .def("GetShaderProgram", &IShaderProgramManager::GetShaderProgram)
            .def("ReleaseShaderProgram", &IShaderProgramManager::ReleaseShaderProgram)
    ];

    luabind::module(L)
    [
        luabind::class_<ShaderProgramHandle>("ShaderProgramHandle")
            .def("Get", (ShaderProgram* (ShaderProgramHandle::*)(void))&ShaderProgramHandle::Get)
    ];

    luabind::module(L)
    [
        luabind::class_<IModelGeometryManager>("ModelGeometryManager")
            .def("GetModelGeometry", &IModelGeometryManager::GetModelGeometry)
            .def("ReleaseModelGeometry", &IModelGeometryManager::ReleaseModelGeometry)
    ];

    luabind::module(L)
    [
        luabind::class_<ModelGeometryHandle>("ModelGeometryHandle")
    ];


    luabind::module(L)
    [
        luabind::class_<ScriptSource>("ScriptSource")
            .def(luabind::constructor<>())
            .def("GetSource", &ScriptSource::GetSource)
            .def("Load", &ScriptSource::Load)
            .def("Dispose", &ScriptSource::Dispose)
    ];

    Logger::Log(1, "Qt Content Manager initialized");
    return true;
}

bool QtContentManager::Release() {

    delete modelGeometryManager;
    delete textureManager;
    delete soundManager;
    delete scriptSourceManager;
    delete shaderProgramManager;
    delete shaderManager;



    textureManager = NULL;
    soundManager = NULL;
    scriptSourceManager = NULL;
    shaderManager = NULL;
    shaderProgramManager = NULL;
    modelGeometryManager = NULL;


    singleton = NULL;
    Logger::Log(1, "Qt Content Manager released");
    return true;
}
