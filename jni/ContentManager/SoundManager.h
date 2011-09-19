#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "Handle.h"
#include "HandleManager.h"
#include "ISoundManager.h"
#include <map>
#include <string>

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

OOLUA_PROXY_CLASS(SoundManager,ISoundManager)
    OOLUA_NO_TYPEDEFS
    OOLUA_ONLY_DEFAULT_CONSTRUCTOR
OOLUA_CLASS_END
#endif // SOUNDMANAGER_H
