#include "VirtualDPad.h"
#include "../Math/Vector2.h"
#include <GLES/gl.h>

VirtualDPad::VirtualDPad(float posX, float posY, float radius, float deadZone)
{
    centerX = posX;
    centerY = posY;
    this->radius = radius;
    this->deadZone = deadZone;

    for(int i=0;i<4;i++)pressed[i]=false;

    pressedAny = false;
}


bool VirtualDPad::IsHit(float x, float y) {
    float dx, dy;
    dx = x - centerX;
    dy = y - centerY;

    float dist = dx*dx + dy*dy;

    if(dist<radius*radius)
        return true;
    return false;
}


bool VirtualDPad::NewEvent(const TouchEvent &event) {
    if(IsHit(event.posX, event.posY)) {
        if(!pressedAny) {
            if(event.action == ENGINE_TOUCHACTION_DOWN || event.action == ENGINE_TOUCHACTION_MOVE) {
                pressedAny = true;
                pressPositionX = event.posX;
                pressPositionY = event.posY;
                pointerId = event.pointerId;
            }
        }
        else {
            if(event.action == ENGINE_TOUCHACTION_UP && event.pointerId == pointerId) {
                pressedAny = false;
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
            pressedAny = false;
            pointerId = -1;
            return true;
        }
    }
    return false;
}

vector<KeyEvent> VirtualDPad::GetEvents() {
    vector<KeyEvent> events;

    if(pressedAny) {
        Vector2 diff(pressPositionX - centerX, pressPositionY - centerY);


        if(diff.y > deadZone) {
            pressed[2] = true;
            KeyEvent e;
            e.action = ENGINE_KEYACTION_DOWN;
            e.keyCode = ENGINE_KEYCODE_DOWN;
            events.push_back(e);

            if(pressed[0]) {
                pressed[0] = false;
                KeyEvent e;
                e.action = ENGINE_KEYACTION_UP;
                e.keyCode = ENGINE_KEYCODE_UP;
                events.push_back(e);
            }
        }
        else if(diff.y < -deadZone) {
            pressed[0] = true;
            KeyEvent e;
            e.action = ENGINE_KEYACTION_DOWN;
            e.keyCode = ENGINE_KEYCODE_UP;
            events.push_back(e);

            if(pressed[2]) {
                pressed[2] = false;
                KeyEvent e;
                e.action = ENGINE_KEYACTION_UP;
                e.keyCode = ENGINE_KEYCODE_DOWN;
                events.push_back(e);
            }
        }
        else {
            if(pressed[0]) {
                pressed[0] = false;
                KeyEvent e;
                e.action = ENGINE_KEYACTION_UP;
                e.keyCode = ENGINE_KEYCODE_UP;
                events.push_back(e);
            }
            if(pressed[2]) {
                pressed[2] = false;
                KeyEvent e;
                e.action = ENGINE_KEYACTION_UP;
                e.keyCode = ENGINE_KEYCODE_DOWN;
                events.push_back(e);
            }
        }


        if(diff.x > deadZone) {
            pressed[1] = true;
            KeyEvent e;
            e.action = ENGINE_KEYACTION_DOWN;
            e.keyCode = ENGINE_KEYCODE_RIGHT;
            events.push_back(e);

            if(pressed[3]) {
                pressed[3] = false;
                KeyEvent e;
                e.action = ENGINE_KEYACTION_UP;
                e.keyCode = ENGINE_KEYCODE_LEFT;
                events.push_back(e);
            }
        }
        else if(diff.x < -deadZone) {
            pressed[3] = true;
            KeyEvent e;
            e.action = ENGINE_KEYACTION_DOWN;
            e.keyCode = ENGINE_KEYCODE_LEFT;
            events.push_back(e);

            if(pressed[1]) {
                pressed[1] = false;
                KeyEvent e;
                e.action = ENGINE_KEYACTION_UP;
                e.keyCode = ENGINE_KEYCODE_RIGHT;
                events.push_back(e);
            }
        }
        else {
            if(pressed[1]) {
                pressed[1] = false;
                KeyEvent e;
                e.action = ENGINE_KEYACTION_UP;
                e.keyCode = ENGINE_KEYCODE_RIGHT;
                events.push_back(e);
            }
            if(pressed[3]) {
                pressed[3] = false;
                KeyEvent e;
                e.action = ENGINE_KEYACTION_UP;
                e.keyCode = ENGINE_KEYCODE_LEFT;
                events.push_back(e);
            }
        }
    }
    else {
        if(pressed[0]) {
            pressed[0] = false;
            KeyEvent e;
            e.action = ENGINE_KEYACTION_UP;
            e.keyCode = ENGINE_KEYCODE_UP;
            events.push_back(e);
        }
        if(pressed[1]) {
            pressed[1] = false;
            KeyEvent e;
            e.action = ENGINE_KEYACTION_UP;
            e.keyCode = ENGINE_KEYCODE_RIGHT;
            events.push_back(e);
        }
        if(pressed[2]) {
            pressed[2] = false;
            KeyEvent e;
            e.action = ENGINE_KEYACTION_UP;
            e.keyCode = ENGINE_KEYCODE_DOWN;
            events.push_back(e);
        }
        if(pressed[3]) {
            pressed[3] = false;
            KeyEvent e;
            e.action = ENGINE_KEYACTION_UP;
            e.keyCode = ENGINE_KEYCODE_LEFT;
            events.push_back(e);
        }
    }

    return events;
}

void VirtualDPad::Draw() {

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


    if(pressedAny) {
        glColor4f(1,0,0,0);
        float ver[8] = {
            pressPositionX - radius/2, pressPositionY + radius/2,
            pressPositionX + radius/2, pressPositionY + radius/2,
            pressPositionX - radius/2, pressPositionY - radius/2,
            pressPositionX + radius/2, pressPositionY - radius/2
          };
        glVertexPointer(2, GL_FLOAT, 0, ver);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}
