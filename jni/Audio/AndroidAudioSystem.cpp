#include "AndroidAudioSystem.h"
#include <Utils/Utils.h>
#include <math.h>

IAudioSystem* IAudioSystem::singleton = NULL;

BufferQueuePlayer AndroidAudioSystem::bufferPlayers[20];
vector<int> AndroidAudioSystem::freeBufferPlayers;

AndroidAudioSystem::AndroidAudioSystem() {
    engineObject=NULL;
    outputMixObject = NULL;
    fdPlayerObject = NULL;
}

AndroidAudioSystem::~AndroidAudioSystem() {

}

bool AndroidAudioSystem::Initialize() {
    ASSERT(!singleton, "Audio system already initialized");

    singleton = this;

    SLresult result;

    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    ASSERT(SL_RESULT_SUCCESS == result, "Cannot create engine object");

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    ASSERT(SL_RESULT_SUCCESS == result, "Cannot realize engine object");

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    ASSERT(SL_RESULT_SUCCESS == result, "Cannot get interface for engine object");

    // create output mix, with environmental reverb specified as a non-required interface
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
    ASSERT(SL_RESULT_SUCCESS == result, "Cannot create output mix");

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    ASSERT(SL_RESULT_SUCCESS == result, "Cannot realize output mix");

    for(int i=0;i<20;i++) {
        CreateBufferQueuePlayer(bufferPlayers[i]);
    }

    freeBufferPlayers.resize(20);
    for(int i=0;i<20;i++) {
        freeBufferPlayers[i]=i;
    }

    Log(1, "Android Audio System system initialized");
    return true;
}



bool AndroidAudioSystem::Release() {
    if (fdPlayerObject != NULL) {
        (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PAUSED);
        (*fdPlayerObject)->Destroy(fdPlayerObject);
        fdPlayerObject = NULL;
        fdPlayerPlay = NULL;
        fdPlayerSeek = NULL;
    }

    // destroy output mix object, and invalidate all associated interfaces
    if (outputMixObject != NULL) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = NULL;
    }

    // destroy engine object, and invalidate all associated interfaces
    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }

    for(int i=0;i<20;i++) {
        if(bufferPlayers[i].bqPlayerObject != NULL) {
            (*bufferPlayers[i].bqPlayerObject)->Destroy(bufferPlayers[i].bqPlayerObject);
            bufferPlayers[i].bqPlayerBufferQueue = NULL;
            bufferPlayers[i].bqPlayerObject = NULL;
            bufferPlayers[i].bqPlayerPlay = NULL;
            bufferPlayers[i].bqPlayerVolume = NULL;
        }
    }

    singleton = NULL;


    Log(1, "Android Audio System system released");
    return true;
}


void AndroidAudioSystem::CreateBufferQueuePlayer(BufferQueuePlayer &player) {
    SLresult result;
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_44_1,
        SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    const SLInterfaceID ids2[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req2[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &player.bqPlayerObject,
                                                &audioSrc, &audioSnk, 3, ids2, req2);
    //assert(SL_RESULT_SUCCESS == result);

    // realize the player
    result = (*player.bqPlayerObject)->Realize(player.bqPlayerObject, SL_BOOLEAN_FALSE);
   // assert(SL_RESULT_SUCCESS == result);

    // get the play interface
    result = (*player.bqPlayerObject)->GetInterface(player.bqPlayerObject, SL_IID_PLAY, &player.bqPlayerPlay);
   // assert(SL_RESULT_SUCCESS == result);

    // get the buffer queue interface
    result = (*player.bqPlayerObject)->GetInterface(player.bqPlayerObject, SL_IID_BUFFERQUEUE,
            &player.bqPlayerBufferQueue);

    result = (*player.bqPlayerObject)->GetInterface(player.bqPlayerObject, SL_IID_VOLUME,
            &player.bqPlayerVolume);
    //assert(SL_RESULT_SUCCESS == result);

    // register callback on the buffer queue
   result = (*player.bqPlayerBufferQueue)->RegisterCallback(player.bqPlayerBufferQueue, BufferPlayerCallback, NULL);
   // assert(SL_RESULT_SUCCESS == result);

    result = (*player.bqPlayerPlay)->SetPlayState(player.bqPlayerPlay, SL_PLAYSTATE_PLAYING);
}


void AndroidAudioSystem::BufferPlayerCallback(SLAndroidSimpleBufferQueueItf fb, void *context) {
    int i=-1;
    for(i=0;i<20;i++) {
        if(fb == bufferPlayers[i].bqPlayerBufferQueue)
            break;
    }

    if(i<20) {
        freeBufferPlayers.push_back(i);
    }
}

void AndroidAudioSystem::PlaySound(const ISound *s, F32 volume) {
    if(freeBufferPlayers.size()==0)return;

    int freePlayer = freeBufferPlayers.back();
    freeBufferPlayers.pop_back();

    volume = 1.0f - volume;
    volume = pow(volume, 5);

    (*bufferPlayers[freePlayer].bqPlayerVolume)->SetVolumeLevel(bufferPlayers[freePlayer].bqPlayerVolume, (I16)(volume*(SL_MILLIBEL_MIN)));

    (*bufferPlayers[freePlayer].bqPlayerBufferQueue)->Enqueue(bufferPlayers[freePlayer].bqPlayerBufferQueue, s->GetData(), s->GetDataLength());
}

void AndroidAudioSystem::PlaySound(const SoundHandle& handle, F32 volume) {
    const ISound* s = handle.Get();
    if(s!=NULL)
        PlaySound(s, volume);
}

void AndroidAudioSystem::PlayMusic(const char *filename, F32 volume) {
    if (fdPlayerObject != NULL) {
        (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PAUSED);
        (*fdPlayerObject)->Destroy(fdPlayerObject);
        fdPlayerObject = NULL;
        fdPlayerPlay = NULL;
        fdPlayerSeek = NULL;
    }

    SLresult result;

    char fileURI[255];
    sprintf(&fileURI[0], "file://%s", filename);

    Log("filename %s", fileURI);
    SLDataLocator_URI loc_uri = {SL_DATALOCATOR_URI, (SLchar*)fileURI};
    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&loc_uri, &format_mime};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    const SLInterfaceID ids[2] = {SL_IID_SEEK, SL_IID_VOLUME};
    const SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &fdPlayerObject, &audioSrc, &audioSnk,
            2, ids, req);
    ASSERT(SL_RESULT_SUCCESS == result, "cannot create audio player");

    // realize the player
    result = (*fdPlayerObject)->Realize(fdPlayerObject, SL_BOOLEAN_FALSE);
    ASSERT(SL_RESULT_SUCCESS == result, "cannot realize audio player");

    // get the play interface
    result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_PLAY, &fdPlayerPlay);
    ASSERT(SL_RESULT_SUCCESS == result, "cannot get interface of player play");

    // get the seek interface
    result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_SEEK, &fdPlayerSeek);
    ASSERT(SL_RESULT_SUCCESS == result, "cannot get interface of player seek");

    // get the volume interface
    result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_VOLUME, &fdPlayerVolume);
    ASSERT(SL_RESULT_SUCCESS == result, "cannot get interface of player volume");

    // enable whole file looping
    result = (*fdPlayerSeek)->SetLoop(fdPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
    ASSERT(SL_RESULT_SUCCESS == result, "canno set loop");

    (*fdPlayerVolume)->GetMaxVolumeLevel(fdPlayerVolume, (SLmillibel*)&maxVolumeLevel);

    volume = 1.0f - volume;
    volume = pow(volume, 5);

    (*fdPlayerVolume)->SetVolumeLevel(fdPlayerVolume, (I16)(volume*(SL_MILLIBEL_MIN)));

    (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PLAYING);
}

void AndroidAudioSystem::SetMusicVolume(F32 volume) {
    if (fdPlayerObject != NULL) {

        volume = 1.0f - volume;
        volume = pow(volume, 5);

        (*fdPlayerVolume)->SetVolumeLevel(fdPlayerVolume, (I16)(volume*(SL_MILLIBEL_MIN)));
    }
}

void AndroidAudioSystem::StopMusic() {
    if (fdPlayerObject != NULL) {
        (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PAUSED);
    }
}
