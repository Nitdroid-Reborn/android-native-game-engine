#ifndef SOUNDFACTORY_H
#define SOUNDFACTORY_H
#include "ISoundFactory.h"

//! Factory of Sound objects
class SoundFactory : public ISoundFactory
{
public:
    SoundFactory();

    ISound* Create();
};

#endif // SOUNDFACTORY_H
