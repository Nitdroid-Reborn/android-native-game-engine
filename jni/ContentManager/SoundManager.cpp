#include "SoundManager.h"
#include <Audio/SoundFactory.h>

SoundManager::SoundManager() : ISoundManager()
{
    SoundHandle::manager = &handleManager;
    handleManager.SetDataFactory(new SoundFactory());
}

SoundHandle SoundManager::GetSound(const char *filename) {

    if(loadedSounds.count(filename)!=0) {
        return loadedSounds[filename];
    }
    SoundHandle handle;
    ISound* sound = handleManager.Acquire(handle);
    sound->Load(filename);

    loadedSounds[filename]=handle;

    return handle;
}

void SoundManager::ReleaseSound(SoundHandle &handle) {
    handleManager.Release(handle);
}


/*EXPORT_OOLUA_FUNCTIONS_NON_CONST(SoundHandle, Get)
EXPORT_OOLUA_FUNCTIONS_CONST(SoundHandle)

EXPORT_OOLUA_FUNCTIONS_NON_CONST(ISoundManager, GetSound, ReleaseSound)
EXPORT_OOLUA_FUNCTIONS_CONST(ISoundManager)*/
