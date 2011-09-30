#include "QtAudioSystem.h"

IAudioSystem* IAudioSystem::singleton = NULL;

QtAudioSystem::QtAudioSystem()
{
}

QtAudioSystem::~QtAudioSystem() {

}


bool QtAudioSystem::Initialize() {
    ASSERT(!singleton, "Audio system already initialized");

    singleton = this;

    //register in lua
    ScriptManager* manager = ScriptManager::Get();
    lua_State* L = manager->getState();

    luabind::module(L)
    [
        luabind::class_<ISound>("Sound")
            .def("Play", &ISound::Play)
    ];

    luabind::module(L)
    [
        luabind::class_<IAudioSystem>("AudioSystem")
            .def("PlayMusic", &IAudioSystem::PlayMusic)
            .def("StopMusic", &IAudioSystem::StopMusic)
            .def("SetMusicVolume", &IAudioSystem::SetMusicVolume)
            .def("PlaySound", ( void (IAudioSystem::*)(const ISound*, F32))&IAudioSystem::PlaySound)
            .def("PlaySound", ( void (IAudioSystem::*)(const SoundHandle&, F32))&IAudioSystem::PlaySound)
            .scope
            [
                luabind::def("Get", IAudioSystem::get)
            ]
    ];


    Logger::Log(1, "Qt Audio System system initialized");
    return true;
}

bool QtAudioSystem::Release() {
    singleton = NULL;


    Logger::Log(1, "Qt Audio System system released");
    return true;
}

void QtAudioSystem::PlayMusic(const char *filename, F32 volume) {

}

void QtAudioSystem::StopMusic() {

}

void QtAudioSystem::SetMusicVolume(F32 volume) {

}

void QtAudioSystem::PlaySound(const ISound *s, F32 volume) {

}

void QtAudioSystem::PlaySound(const SoundHandle &handle, F32 volume) {

}
