#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "Handle.h"
#include "HandleManager.h"
#include "ISoundManager.h"
#include <map>
#include <string>

//! Implementation of sounde manager
class SoundManager : public ISoundManager
{
    SoundHandleManager handleManager;

public:
    SoundManager();
    ~SoundManager() {

    }

    SoundHandle GetSound(const char *filename);
    void ReleaseSound(SoundHandle&);

private:
    std::map<std::string, SoundHandle> loadedSounds;
};
#endif // SOUNDMANAGER_H
