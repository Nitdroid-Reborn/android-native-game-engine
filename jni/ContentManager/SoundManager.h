#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "Handle.h"
#include "HandleManager.h"
#include "ISoundManager.h"

class SoundManager : public ISoundManager
{
    SoundHandleManager handleManager;

public:
    SoundManager();
    ~SoundManager(){}

    SoundHandle GetSound(const char *filename);
    void ReleaseSound(SoundHandle&);
};

#endif // SOUNDMANAGER_H
