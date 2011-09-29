#ifndef WAVESOUND_H
#define WAVESOUND_H
#include <Utils/EngineTypes.h>

//! Wave format support.
/*!
   Currently sounds should be mono with 16 bits values and 44kHz sampling.
  */
class WaveSound
{
public:
    WaveSound();
    ~WaveSound();

    //! Loads wav file
    /*!
      \param filename path to wav file
      \param data data buffer to which will be written raw sound data, it will be allocated by this function
      \param sampleCount number of samples written to data buffer
      */
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
