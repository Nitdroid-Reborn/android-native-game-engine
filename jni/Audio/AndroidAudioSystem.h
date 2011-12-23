#ifdef ANDROID
#ifndef ANDROIDAUDIOSYSTEM_H
#define ANDROIDAUDIOSYSTEM_H

#include "IAudioSystem.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <vector>

using namespace std;

//contains all object/interfaces for one sound player
struct BufferQueuePlayer {
    SLObjectItf bqPlayerObject;
    SLPlayItf bqPlayerPlay;
    SLVolumeItf bqPlayerVolume;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
};

//! Implementation of IAudioSystem using OpenSL ES
/*!
  Can play up to 20 sound in the same time.
  */
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

    void PlaySoundLoop(const ISound*s, F32 volume);
    void PlaySoundLoop(const SoundHandle&h, F32 volume);
    void StopSoundLoop();

private:
    //callback of buffer playback end, it marks music player as free
    static void BufferPlayerCallback(SLAndroidSimpleBufferQueueItf fb, void* context);
    // create single music player
    void CreateBufferQueuePlayer(BufferQueuePlayer& player);

    //main sound engine object and interface
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    // output mix interfaces
    SLObjectItf outputMixObject;

    // file descriptor player interfaces
    SLObjectItf fdPlayerObject;
    SLPlayItf fdPlayerPlay;
    SLSeekItf fdPlayerSeek;
    SLVolumeItf fdPlayerVolume;

    //sound players
    static BufferQueuePlayer bufferPlayers[20];
    static BufferQueuePlayer loopPlayer;
    //list of free sound players
    static vector<int> freeBufferPlayers;

    static const ISound* loopedSound;
    static bool loop;

    U16 maxVolumeLevel;
};

#endif // ANDROIDAUDIOSYSTEM_H
#endif
