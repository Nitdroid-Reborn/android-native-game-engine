#ifndef ISOUND_H
#define ISOUND_H

#include <Utils/EngineTypes.h>


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

#endif // ISOUND_H
