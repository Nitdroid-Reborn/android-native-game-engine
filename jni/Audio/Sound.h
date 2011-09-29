#ifndef SOUND_H
#define SOUND_H
#include "ISound.h"

//! Implementation of ISound interface.
/*!
  Currently sounds must be mono with 16 bits values and 44kHz sampling.
*/
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
#endif // SOUND_H
