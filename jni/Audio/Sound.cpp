#include "Sound.h"
#include "WaveSound.h"
#include <Utils/Utils.h>
#include <Audio/IAudioSystem.h>

Sound::Sound() : ISound() {
    data=0;
    sampleCount=0;
}

Sound::~Sound() {
    if(data!=NULL)
        Dispose();
}

bool Sound::Load(const char *filename) {
    //load using WaveSound object
    WaveSound s;
    return s.Load(filename, data, sampleCount);
}

void Sound::Dispose() {
    Logger::Log(0, "Sound disposed");
    delete[] data;
    data = NULL;
    sampleCount = 0;
}

void Sound::Play(float volume) {
    //get audio system and play sound
    IAudioSystem::get()->PlaySound(this, volume);
}

const U16* Sound::GetData() const {
    return (const U16*)data;
}

const U32 Sound::GetDataLength() const {
    return sampleCount*2;
}
