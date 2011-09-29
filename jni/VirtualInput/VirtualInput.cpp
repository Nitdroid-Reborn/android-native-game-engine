#include "VirtualInput.h"

VirtualInput::VirtualInput()
{
    numKeys = 0;
}


void VirtualInput::AddKey(VirtualKey *key) {
    if(numKeys < 6) {
        keys[numKeys++] = key;
    }
}

bool VirtualInput::NewTouchEvent(const TouchEvent &event) {
    for(int i=0;i<numKeys;i++) {
        if(keys[i]->NewEvent(event))
            return true;
    }
}

void VirtualInput::Draw() {
    for(int i=0;i<numKeys;i++) {
        keys[i]->Draw();
    }
}

vector<KeyEvent> VirtualInput::GetEvents() {
    vector<KeyEvent> events;
    for(int i=0;i<numKeys;i++) {
        vector<KeyEvent> kEv = keys[i]->GetEvents();
        for(int i=0;i<kEv.size();++i)
            events.push_back(kEv[i]);
    }
    return events;
}
