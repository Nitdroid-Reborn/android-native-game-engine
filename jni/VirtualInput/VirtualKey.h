#ifndef VIRTUALKEY_H
#define VIRTUALKEY_H
#include "Input/InputKeys.h"

#include <vector>
using namespace std;

class VirtualKey
{
public:
    VirtualKey(){};

    virtual bool NewEvent(const TouchEvent& event)=0;
    virtual vector<KeyEvent> GetEvents()=0;
    virtual void Draw()=0;
};

#endif // VIRTUALKEY_H
