#ifndef VIRTUALSINGLEKEY_H
#define VIRTUALSINGLEKEY_H

#include "VirtualKey.h"


class VirtualSingleKey : public VirtualKey
{
public:
    VirtualSingleKey(EngineKeyCode keyCode, float posX, float posY, float radius);
    bool NewEvent(const TouchEvent &event);
    vector<KeyEvent> GetEvents();
    void Draw();



private:
    bool IsHit(float x, float y);


    float centerX;
    float centerY;
    float radius;
    bool pressed;
    bool oldPressed;
    float pressPositionX;
    float pressPositionY;

    EngineKeyCode keyCode;


    int pointerId;
};

#endif // VIRTUALSINGLEKEY_H
