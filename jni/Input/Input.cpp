#include "Input.h"

Input::Input()
{
}


const KeysState* Input::GetKeyState() const {
    return &keyState;
}

void Input::ProcessKeyEvent(const KeyEvent &event) {
    keyState.NewEvent(event);
    if(event.action==ENGINE_KEYACTION_DOWN) {

        //LOGI("Touch event: down, code %llu", event.keyCode);
        //if(event.keyCode == ENGINE_KEYCODE_G)
          //  LOGI("ok");
    }
    else if(event.action == ENGINE_KEYACTION_UP) {
    }
      //  LOGI("Touch event: up, code %d", event.keyCode);
}

void Input::ProcessTouchEvent(const TouchEvent &event) {
    if(event.action==ENGINE_TOUCHACTION_DOWN) {
       // LOGI("Touch event: down, id %d posx %f, posy %f", event.pointerId, event.posX, event.posY);

    }
    else if(event.action == ENGINE_TOUCHACTION_UP) {
      //  LOGI("Touch event: up, id %d posx %f, posy %f", event.pointerId, event.posX, event.posY);

    }
    else {
      //  LOGI("Touch event: move, id %d posx %f, posy %f", event.pointerId, event.posX, event.posY);

    }
}


void Input::EndFrame() {
    keyState.EndFrame();
}

void Input::StartFrame() {
    keyState.DetectKeyUpDownEvents();
}
