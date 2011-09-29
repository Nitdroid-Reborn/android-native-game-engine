#ifndef ISOUNDFACTORY_H
#define ISOUNDFACTORY_H
#include <Audio/ISound.h>

//! Abstract factory of ISound objects.
class ISoundFactory {
public:
    ISoundFactory(){}
    //! Creates new ISound object
    virtual ISound* Create()=0;
};

#endif // ISOUNDFACTORY_H
