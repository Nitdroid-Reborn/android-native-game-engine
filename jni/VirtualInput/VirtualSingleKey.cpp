#include "VirtualSingleKey.h"
#include <Utils/Utils.h>
#include <GLES/gl.h>

VirtualSingleKey::VirtualSingleKey(EngineKeyCode keyCode, float posX, float posY, float radius)
{
    this->keyCode = keyCode;
    centerX = posX;
    centerY = posY;
    this->radius = radius;

    pressed = false;
    oldPressed = false;
}


bool VirtualSingleKey::IsHit(float x, float y) {
    float dx, dy;
    dx = x - centerX;
    dy = y - centerY;

    float dist = dx*dx + dy*dy;

    if(dist<radius*radius)
        return true;
    return false;
}


bool VirtualSingleKey::NewEvent(const TouchEvent &event) {
    if(IsHit(event.posX, event.posY)) {
        if(!pressed) {
            if(event.action == ENGINE_TOUCHACTION_DOWN || event.action == ENGINE_TOUCHACTION_MOVE) {
                pressed = true;
                pressPositionX = event.posX;
                pressPositionY = event.posY;
                pointerId = event.pointerId;
            }
        }
        else {
            if(event.action == ENGINE_TOUCHACTION_UP && event.pointerId == pointerId) {
                pressed = false;
                pointerId = -1;
                pressPositionX = event.posX;
                pressPositionY = event.posY;
            }
            else if(event.action == ENGINE_TOUCHACTION_MOVE && event.pointerId == pointerId) {
                pressPositionX = event.posX;
                pressPositionY = event.posY;
            }
        }
        return true;
    }
    else {

        if(event.pointerId == pointerId && pressed) {
            pressed = false;
            pointerId = -1;

            return true;
        }
    }
    return false;
}

vector<KeyEvent> VirtualSingleKey::GetEvents() {
    vector<KeyEvent> events;
    if(pressed) {
        KeyEvent e;
        e.keyCode = keyCode;
        e.action = ENGINE_KEYACTION_DOWN;
        events.push_back(e);
    }
    else if(!pressed && oldPressed) {
        KeyEvent e;
        e.keyCode = keyCode;
        e.action = ENGINE_KEYACTION_UP;
        events.push_back(e);
    }

    oldPressed = pressed;
    return events;
}

void VirtualSingleKey::Draw() {
    if(pressed)
        glColor4f(1,0,0,1 );
    else
        glColor4f(1,1,1,1);

    float ver[8] = {
        centerX - radius, centerY + radius,
        centerX + radius, centerY + radius,
        centerX - radius, centerY - radius,
        centerX + radius, centerY - radius
      };

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, ver);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}
