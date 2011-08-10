#ifndef VIRTUALINPUT_H
#define VIRTUALINPUT_H

#include <vector>
#include "VirtualKey.h"
#include "Input/InputKeys.h"

using namespace std;

class VirtualInput
{
public:
    VirtualInput();

    vector<KeyEvent> GetEvents();
    bool NewTouchEvent(const TouchEvent& event);
    void AddKey(VirtualKey* key);

private:
    VirtualKey* keys[6];
    unsigned int numKeys;
};

#endif // VIRTUALINPUT_H
