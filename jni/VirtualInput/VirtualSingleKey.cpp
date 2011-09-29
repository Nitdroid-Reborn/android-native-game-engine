#include "VirtualSingleKey.h"
#include <Utils/Utils.h>
#include <Graphics/IRenderer.h>

VirtualSingleKey::VirtualSingleKey(EngineKeyCode keyCode, float posX, float posY, float radius)
{
    this->keyCode = keyCode;
    centerX = posX;
    centerY = posY;
    this->radius = radius;

    pressed = false;
    oldPressed = false;

    texture = IContentManager::get()->GetTextureManager()->GetTexture("dpad.png");
}

VirtualSingleKey::~VirtualSingleKey() {
    IContentManager::get()->GetTextureManager()->ReleaseTexture(texture);
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
    if(pressed) {
        TextureRegion region(63.0/192, 35.0/192, 126.0/192, 97.0/192);
        IRenderer::get()->DrawSprite(centerX, centerY, VIRTUAL_INPUT_LAYER, 2*radius, 2*radius, region, texture);
    }
    else {
        TextureRegion region(0, 0.177, 0.333, 0.5104);
        IRenderer::get()->DrawSprite(centerX, centerY, VIRTUAL_INPUT_LAYER, 2*radius, 2*radius, region, texture);
    }
}
