#ifndef VIRTUALDPAD_H
#define VIRTUALDPAD_H
#include "VirtualKey.h"


class VirtualDPad : public VirtualKey
{
public:
    VirtualDPad(float posX, float posY, float radius, float deadZone);
    bool NewEvent(const TouchEvent &event);
    vector<KeyEvent> GetEvents();
    void Draw();


private:
    bool IsHit(float x, float y);

    float centerX;
    float centerY;
    float radius;
    bool pressed[4];
    bool pressedAny;
    float pressPositionX;
    float pressPositionY;

    int pointerId;
    float deadZone;

    bool oldUpAction[4];
    bool oldDownAction[4];

};

#endif // VIRTUALDPAD_H
