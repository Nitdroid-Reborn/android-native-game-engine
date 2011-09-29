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
