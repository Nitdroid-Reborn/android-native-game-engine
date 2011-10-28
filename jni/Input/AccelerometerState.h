#ifndef ACCELEROMETERSTATE_H
#define ACCELEROMETERSTATE_H

#include "Math/MathLib.h"
#include "InputKeys.h"
#include <vector>

class AccelerometerState
{
public:
    AccelerometerState();

    Vector3 GetAcceleration() const;
    Vector3 GetRawAcceleration() const;

    void NewEvent(const AccelEvent& event);
private:
    Vector3 lastRawData;
    std::vector<Vector3> lastNFrames;
    Vector3 lastSmoothData;
    U16 smoothSize;
};

#endif // ACCELEROMETERSTATE_H
