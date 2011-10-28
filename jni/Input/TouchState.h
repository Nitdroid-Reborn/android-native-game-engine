#ifndef TOUCHSTATE_H
#define TOUCHSTATE_H

#include "Input/InputKeys.h"
#include "Math/MathLib.h"

//! Tracks and updates state of touch
class TouchState
{
public:
    TouchState();

    //! Checks if pointer is down
    /*!
      \param pointerId id of pointer in interest
      */
    bool IsPointerDown(EnginePointerId pointerId) const;

    //! Checks if pointer just went down
    /*!
      \param pointerId id of pointer in interest
      */
    bool IsPointerJustDown(EnginePointerId pointerId) const;

    //! Checks if pointer went up
    /*!
      \param pointerId id of pointer in interest
      */
    bool IsPointerJustUp(EnginePointerId pointerId) const;

    //! Returns x position of pointer
    /*!
      Returns current position only if pointer is down, otherwise return last position when pointer was down.
      \param pointerId id of pointer in interest
      */
    float GetPointerX(EnginePointerId pointerId) const;

    //! Returns y position of pointer
    /*!
      Returns current position only if pointer is down, otherwise return last position when pointer was down.
      \param pointerId id of pointer in interest
      */
    float GetPointerY(EnginePointerId pointerId) const;

    //! Returns position of pointer as Vector2
    /*!
      Returns current position only if pointer is down, otherwise return last position when pointer was down.
      \param pointerId id of pointer in interest
      */
    Vector2 GetPointerPosition(EnginePointerId pointerId) const;

    //! Calculates changes of touch states from last frame
    void StartFrame();

    //! Saves state of touch in this frame
    void EndFrame();

    //! Process touch event and updates touch state
    void NewEvent(const TouchEvent& event);

private:
    void DetectPointerUpDownEvents();
    //touch state is coded in 16-bit int
    //when bit is set pointer is pressed
    //bit of each pointer can be calculated as 1<<pointerId
    U16 pointerStates;
    U16 previousPointerStates;
    U16 pointersDowns;
    U16 pointersUps;
    Vector2 fingersPositions[ENGINE_TOUCH_MAX_POINTERS];
};
#endif // TOUCHSTATE_H
