#ifndef CLOCK_H
#define CLOCK_H
#include "EngineTypes.h"

class Clock
{
public:
    explicit Clock(U64 startMSeconds = 0);
    U64 getMSeconds() const;
    F32 getTimeDelta(const Clock& otherClock);
    void update(U64 realMSeconds);
    void setPaused(bool paused);
    bool isPaused() const;
    void setTimeScale(F32 timeScale);
    F32 getTimeScale() const;
    void reset();

    void singleStep();

private:
    U64 mseconds;
    F32 timeScale;
    bool paused;
};

#endif // CLOCK_H
