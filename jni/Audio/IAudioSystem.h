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


int IAudioSystemGet(lua_State* l);

OOLUA_PROXY_CLASS(IAudioSystem)
    OOLUA_TYPEDEFS Abstract OOLUA_END_TYPES
    OOLUA_MEM_FUNC_2(void, PlayMusic, const char*, F32)
    OOLUA_MEM_FUNC_0(void, StopMusic)
    OOLUA_MEM_FUNC_1(void, SetMusicVolume, F32)
    OOLUA_MEM_FUNC_2(void, PlaySound, const SoundHandle&, F32)

OOLUA_CLASS_END
#endif // IAUDIOSYSTEM_H
