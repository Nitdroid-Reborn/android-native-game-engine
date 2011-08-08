#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <sys/types.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include <android/asset_manager.h>



struct BufferQueuePlayer {
    SLObjectItf bqPlayerObject;
    SLPlayItf bqPlayerPlay;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
};

class AudioSystem
{
public:
    AudioSystem();


    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    // output mix interfaces
    SLObjectItf outputMixObject;

    // file descriptor player interfaces
    SLObjectItf fdPlayerObject;
    SLPlayItf fdPlayerPlay;
    SLSeekItf fdPlayerSeek;

    BufferQueuePlayer bufferPlayers[20];

    void CreateEngine();
    void Shutdown();
    void CreateAssetPlayer(AAssetManager* manager, char* filename);
    void SetAssetPlayerStatus(bool isPlaying);

    void CreateBufferPlayer(BufferQueuePlayer& player);


    void Play(BufferQueuePlayer& player, const char* data, int len);


};

#endif // AUDIOSYSTEM_H
