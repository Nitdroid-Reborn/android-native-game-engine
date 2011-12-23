#ifndef IAUDIOSYSTEM_H
#define IAUDIOSYSTEM_H

#include <Utils/EngineTypes.h>
#include <Audio/ISound.h>
#include <ContentManager/SoundManager.h>

//! Interface for audio subsystem
class IAudioSystem {
public:
    IAudioSystem() {}
    virtual ~IAudioSystem() {}

    //! Initializes subsystem
    /*!
      Place for loading all neccesary resources and register in scipts system
      */
    virtual bool Initialize()=0;

    //! Releases subsystem
    /*!
      Place for release all used resources
      */
    virtual bool Release()=0;

    //! Plays music file
    /*!
      \param filename path to file
      \volume volume of playback in range of 0.0 to 1.0
      */
    virtual void PlayMusic(const char* filename, F32 volume=1.0f)=0;
    //! Stops music playback
    virtual void StopMusic()=0;

    //! Sets music playback volume
    /*!
      \param volume volume of playback in range of 0.0 to 1.0
      */
    virtual void SetMusicVolume(F32 volume)=0;

    //! Plays sound
    /*!
      \param s sound to play
      \param volume volume of sound playback in range of 0.0 to 1.0
      */
    virtual void PlaySound(const ISound*s, F32 volume=1.0f)=0;

    //! Plays sound
    /*!
      \param handle handle of sound to play
      \param volume volume of sound playback in range of 0.0 to 1.0
      */
    virtual void PlaySound(const SoundHandle& handle, F32 volume=1.0f)=0;


    virtual void PlaySoundLoop(const ISound*s, F32 volume)=0;
    virtual void PlaySoundLoop(const SoundHandle&h, F32 volume)=0;
    virtual void StopSoundLoop()=0;


    //! Returns instance of audio subsystem
    static IAudioSystem* get(){return singleton;}

protected:
    static IAudioSystem* singleton;
};
#endif // IAUDIOSYSTEM_H
