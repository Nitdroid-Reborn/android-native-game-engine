#ifndef ANDROIDAUDIOSYSTEM_H
#define ANDROIDAUDIOSYSTEM_H

#include "IAudioSystem.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <vector>

using namespace std;

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
    static void BufferPlayerCallback(SLAndroidSimpleBufferQueueItf fb, void* context);
    void CreateBufferQueuePlayer(BufferQueuePlayer& player);

    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    // output mix interfaces
    SLObjectItf outputMixObject;

    // file descriptor player interfaces
    SLObjectItf fdPlayerObject;
    SLPlayItf fdPlayerPlay;
    SLSeekItf fdPlayerSeek;
    SLVolumeItf fdPlayerVolume;

    static BufferQueuePlayer bufferPlayers[20];
    static vector<int> freeBufferPlayers;


    U16 maxVolumeLevel;
};


/*OOLUA_PROXY_CLASS(AndroidAudioSystem, IAudioSystem)
    OOLUA_NO_TYPEDEFS
    OOLUA_ONLY_DEFAULT_CONSTRUCTOR
OOLUA_CLASS_END*/

#endif // ANDROIDAUDIOSYSTEM_H
