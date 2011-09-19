#ifndef ISOUND_H
#define ISOUND_H

#include <Utils/EngineTypes.h>
#include <Scripts/oolua/oolua.h>

class ISound {
public:
    ISound(){}
    virtual ~ISound(){}

    virtual void Play(float volume=1.0f)=0;
    virtual bool Load(const char* filename)=0;
    virtual void Dispose()=0;
    virtual const U16* GetData() const=0;
    virtual const U32 GetDataLength() const=0;
};

OOLUA_PROXY_CLASS(ISound)
    OOLUA_MEM_FUNC(void, Play, float)
    OOLUA_MEM_FUNC(bool, Load, const char*)
    OOLUA_MEM_FUNC(void, Dispose)
    OOLUA_TYPEDEFS Abstract OOLUA_END_TYPES
OOLUA_CLASS_END

#endif // ISOUND_H
