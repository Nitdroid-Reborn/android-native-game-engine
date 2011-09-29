#ifndef ISOUNDMANAGER_H
#define ISOUNDMANAGER_H

#include "Handle.h"
#include "HandleManager.h"
#include <Audio/ISoundFactory.h>

struct tagSound {};


typedef Handle<ISound, ISoundFactory, tagSound> SoundHandle;
typedef HandleManager<ISound, ISoundFactory, tagSound> SoundHandleManager;

//! Interface for sound manager
class ISoundManager {
public:
    ISoundManager(){}
    virtual ~ISoundManager(){}

    //! Gets sound
    /*!
      If sound is not loaded, loads it to memory, otherwise returns handle to it.
      \param filename path to sound file
    */
    virtual SoundHandle GetSound(const char* filename)=0;

    //! Releases sound
    /*!
      Releases reference to sound from handle, if reference count for this sound drops to 0 deletes it
      \param handle handle to sound
      */
    virtual void ReleaseSound(SoundHandle& handle)=0;
};
#endif // ISOUNDMANAGER_H
