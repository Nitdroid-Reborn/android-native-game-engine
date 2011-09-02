#include "SoundManager.h"
#include <Audio/SoundFactory.h>

SoundManager::SoundManager() : ISoundManager()
{
    SoundHandle::manager = &handleManager;
    handleManager.SetDataFactory(new SoundFactory());
}

SoundHandle SoundManager::GetSound(const char *filename) {
    SoundHandle handle;
    ISound* sound = handleManager.Acquire(handle);
    sound->Load(filename);

    return handle;
}

void SoundManager::ReleaseSound(SoundHandle &handle) {
    handleManager.Release(handle);
}
