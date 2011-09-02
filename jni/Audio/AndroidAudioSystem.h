#ifndef ANDROIDAUDIOSYSTEM_H
#define ANDROIDAUDIOSYSTEM_H

#include "IAudioSystem.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>


struct BufferQueuePlayer {
    SLObjectItf bqPlayerObject;
    SLPlayItf bqPlayerPlay;
    SLVolumeItf bqPlayerVolume;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
};


class AndroidAudioSystem : public IAudioSystem
{
public:
    AndroidAudioSystem();
    ~AndroidAudioSystem();

    bool Initialize();
    bool Release();

    void PlayMusic(const char* filename, F32 volume);
    void StopMusic();
    void SetMusicVolume(F32 volume);

    void PlaySound(const ISound*s, F32 volume);
    void PlaySound(const SoundHandle& handle, F32 volume);

private:
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    // output mix interfaces
    SLObjectItf outputMixObject;

    // file descriptor player interfaces
    SLObjectItf fdPlayerObject;
    SLPlayItf fdPlayerPlay;
    SLSeekItf fdPlayerSeek;
    SLVolumeItf fdPlayerVolume;

    BufferQueuePlayer bufferPlayer;


    U16 maxVolumeLevel;
};

#endif // ANDROIDAUDIOSYSTEM_H
