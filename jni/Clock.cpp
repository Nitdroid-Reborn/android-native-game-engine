#include "Clock.h"

Clock::Clock(U64 startMSeconds)
{
    mseconds = startMSeconds;
    timeScale = 1.0f;
    paused = false;
}

U64 Clock::getMSeconds() const {
    return mseconds;
}

F32 Clock::getTimeDelta(const Clock &otherClock) {
    U64 dt = mseconds - otherClock.mseconds;
    return (F32)dt;
}

void Clock::update(U64 realMSeconds) {
    if(!paused) {
        F32 scaledTime = timeScale * realMSeconds;
        mseconds += (U64)scaledTime;
    }
}

void Clock::setPaused(bool paused) {
    this->paused = paused;
}

bool Clock::isPaused() const {
    return paused;
}

void Clock::setTimeScale(F32 timeScale) {
    this->timeScale = timeScale;
}

F32 Clock::getTimeScale() const {
    return timeScale;
}

void Clock::singleStep() {
    if(paused) {
        F32 scaledTime = timeScale * (1.0f/60.0f);
        mseconds += (U64)scaledTime;
    }
}
