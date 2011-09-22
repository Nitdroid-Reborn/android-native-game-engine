#ifndef TOUCHSTATE_H
#define TOUCHSTATE_H

#include "Input/InputKeys.h"
#include "Math/MathLib.h"
#include <Scripts/ScriptManager.h>

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

/*OOLUA_PROXY_CLASS(TouchState)
    OOLUA_NO_TYPEDEFS
    OOLUA_ONLY_DEFAULT_CONSTRUCTOR
    OOLUA_MEM_FUNC_1_CONST(bool, IsPointerDown, U8)
    OOLUA_MEM_FUNC_1_CONST(bool, IsPointerJustDown, U8)
    OOLUA_MEM_FUNC_1_CONST(bool, IsPointerJustUp, U8)
    OOLUA_MEM_FUNC_1_CONST(float, GetPointerX, U8)
    OOLUA_MEM_FUNC_1_CONST(float, GetPointerY, U8)
OOLUA_CLASS_END
*/
#endif // TOUCHSTATE_H
