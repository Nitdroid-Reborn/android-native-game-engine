#ifndef ISOUND_H
#define ISOUND_H

#include <Utils/EngineTypes.h>

//! Interface of sound
/*!
  Sounds should be small about 5s, there are loaded into memory in opposite to music
  which is streamed.
*/
class ISound {
public:
    ISound(){}
    virtual ~ISound(){}

    //! Plays sound
    /*!
      \param volume volume of sound playback in range of 0.0 to 1.0
      */
    virtual void Play(float volume=1.0f)=0;
    //! Loads sound from file
    /*!
      \param filename path to sound file
      */
    virtual bool Load(const char* filename)=0;
    //! Releases all resources of sound
    virtual void Dispose()=0;

    //! Return raw data of sound
    virtual const U16* GetData() const=0;

    //! Return length of raw data of sound
    virtual const U32 GetDataLength() const=0;
};

#endif // ISOUND_H
