#ifndef ISOUNDFACTORY_H
#define ISOUNDFACTORY_H
#include <Audio/ISound.h>

class ISoundFactory {
public:
    ISoundFactory(){}

    virtual ISound* Create()=0;
};

#endif // ISOUNDFACTORY_H
