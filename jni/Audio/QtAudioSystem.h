#ifndef QTAUDIOSYSTEM_H
#define QTAUDIOSYSTEM_H
#include "IAudioSystem.h"


class QtAudioSystem : public IAudioSystem
{
public:
    QtAudioSystem();
    ~QtAudioSystem();

    bool Initialize();
    bool Release();

    void PlayMusic(const char* filename, F32 volume);
    void StopMusic();
    void SetMusicVolume(F32 volume);

    void PlaySound(const ISound*s, F32 volume);
    void PlaySound(const SoundHandle& handle, F32 volume);

    void PlaySoundLoop(const ISound *s, F32 volume);
    void PlaySoundLoop(const SoundHandle&h, F32 volume);
    void StopSoundLoop();

private:

};

#endif // QTAUDIOSYSTEM_H
