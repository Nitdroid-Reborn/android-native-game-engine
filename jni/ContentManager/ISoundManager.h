#ifndef ISOUNDMANAGER_H
#define ISOUNDMANAGER_H

#include "Handle.h"
#include "HandleManager.h"
#include <Audio/ISoundFactory.h>

struct tagSound {};

typedef Handle<ISound, ISoundFactory, tagSound> SoundHandle;
typedef HandleManager<ISound, ISoundFactory, tagSound> SoundHandleManager;

class ISoundManager {
public:
    ISoundManager(){}
    virtual ~ISoundManager(){}

    virtual SoundHandle GetSound(const char* filename)=0;
    virtual void ReleaseSound(SoundHandle& handle)=0;
};


#endif // ISOUNDMANAGER_H
