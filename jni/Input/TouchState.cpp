#include "TouchState.h"

TouchState::TouchState() {
    pointerStates = 0x0;
    previousPointerStates = 0x0;
}

void TouchState::DetectPointerUpDownEvents() {
    U16 pointersChanges = pointerStates ^ previousPointerStates;

    pointersDowns = pointersChanges & pointerStates;
    pointersUps = pointersChanges & (~pointerStates);
}

bool TouchState::IsPointerDown(EnginePointerId fingerId) const {
    return (pointerStates & (1<<fingerId));
}

bool TouchState::IsPointerJustDown(EnginePointerId pointerId) const {
    return (pointersDowns & (1<<pointerId));
}

bool TouchState::IsPointerJustUp(EnginePointerId pointerId) const {
    return (pointersUps & (1<<pointerId));
}

float TouchState::GetPointerX(EnginePointerId fingerId) const {
    return fingersPositions[fingerId].x;
}

float TouchState::GetPointerY(EnginePointerId fingerId) const {
    return fingersPositions[fingerId].y;
}

Vector2 TouchState::GetPointerPosition(EnginePointerId fingerId) const {
    return fingersPositions[fingerId];
}


void TouchState::NewEvent(const TouchEvent &event) {
    if(event.action == ENGINE_TOUCHACTION_DOWN) {
        pointerStates |= (1<<event.pointerId);
        fingersPositions[event.pointerId] = Vector2(event.posX, event.posY);
    }
    else if(event.action == ENGINE_TOUCHACTION_UP) {
        pointerStates = pointerStates & (~(1<<event.pointerId));
        fingersPositions[event.pointerId] = Vector2(event.posX, event.posY);
    }
    else {
        fingersPositions[event.pointerId] = Vector2(event.posX, event.posY);
    }
}

void TouchState::StartFrame() {
    DetectPointerUpDownEvents();
}

void TouchState::EndFrame() {
    previousPointerStates = pointerStates;
}

//EXPORT_OOLUA_FUNCTIONS_NON_CONST(TouchState)
//EXPORT_OOLUA_FUNCTIONS_CONST(TouchState, IsPointerDown, IsPointerJustDown, IsPointerJustUp, GetPointerX, GetPointerY)
