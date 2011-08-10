#ifndef TOUCHSTATE_H
#define TOUCHSTATE_H

#include "Input/InputKeys.h"
#include "Math/MathLib.h"


class TouchState
{
public:
    TouchState();

    bool IsPointerDown(EnginePointerId pointerId) const;
    bool IsPointerJustDown(EnginePointerId pointerId) const;
    bool IsPointerJustUp(EnginePointerId pointerId) const;
    float GetPointerX(EnginePointerId pointerId) const;
    float GetPointerY(EnginePointerId pointerId) const;
    Vector2 GetPointerPosition(EnginePointerId pointerId) const;

    void StartFrame();
    void EndFrame();

    void NewEvent(const TouchEvent& event);

private:
    void DetectPointerUpDownEvents();
    U16 pointerStates;
    U16 previousPointerStates;
    U16 pointersDowns;
    U16 pointersUps;
    Vector2 fingersPositions[ENGINE_TOUCH_MAX_POINTERS];
};

#endif // TOUCHSTATE_H
