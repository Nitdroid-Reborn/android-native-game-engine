#ifndef SOUND_H
#define SOUND_H
#include "ISound.h"

class Sound : public ISound
{
public:
    Sound();
    ~Sound();

    bool Load(const char *filename);
    void Dispose();
    void Play(float volume);
    const U16* GetData() const;
    const U32 GetDataLength() const;

private:
    U16* data;
    U32 sampleCount;
};

OOLUA_PROXY_CLASS(Sound,ISound)
    OOLUA_NO_TYPEDEFS
    OOLUA_ONLY_DEFAULT_CONSTRUCTOR
OOLUA_CLASS_END

#endif // SOUND_H
