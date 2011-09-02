#ifndef IAUDIOSYSTEM_H
#define IAUDIOSYSTEM_H

#include <Utils/EngineTypes.h>
#include <Audio/ISound.h>
#include <ContentManager/SoundManager.h>

class IAudioSystem {
public:
    IAudioSystem() {}
    virtual ~IAudioSystem() {}

    virtual bool Initialize()=0;
    virtual bool Release()=0;

    virtual void PlayMusic(const char* filename, F32 volume=1.0f)=0;
    virtual void StopMusic()=0;
    virtual void SetMusicVolume(F32 volume)=0;

    virtual void PlaySound(const ISound*s, F32 volume=1.0f)=0;
    virtual void PlaySound(const SoundHandle& handle, F32 volume=1.0f)=0;


    static IAudioSystem* get(){return singleton;}

protected:
    static IAudioSystem* singleton;
};

#endif // IAUDIOSYSTEM_H
