#include "SoundFactory.h"
#include "Sound.h"

SoundFactory::SoundFactory()
{
}

ISound* SoundFactory::Create() {
    return new Sound();
}
