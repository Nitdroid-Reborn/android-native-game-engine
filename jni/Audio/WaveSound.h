#ifndef WAVESOUND_H
#define WAVESOUND_H
#include <Utils/EngineTypes.h>

class WaveSound
{
public:
    WaveSound();
    ~WaveSound();

    bool Load(const char* filename, U16*& data, U32& sampleCount);

private:
    U16 compressionCode;
    U16 channels;
    U32 sampleRate;
    U32 avgBytesPerSecond;
    U16 blockAlign;
    U16 significantBitsPerSample;
    U32 fileSize;
    U32 dataSize;
};

#endif // WAVESOUND_H
