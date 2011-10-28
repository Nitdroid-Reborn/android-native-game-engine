#include "AccelerometerState.h"

AccelerometerState::AccelerometerState()
{
    smoothSize = 10;
    lastRawData.Zero();
    lastSmoothData.Zero();
}

Vector3 AccelerometerState::GetAcceleration() const {
    return lastSmoothData;
}

Vector3 AccelerometerState::GetRawAcceleration() const {
    return lastRawData;
}

void AccelerometerState::NewEvent(const AccelEvent &event) {
    Vector3 newData(event.x, event.y, event.z);

    lastRawData = newData;

    lastNFrames.push_back(newData);
    if(lastNFrames.size()>smoothSize) {
        lastNFrames.erase(lastNFrames.begin());
    }

    lastSmoothData.Zero();
    for(int i=0;i<lastNFrames.size();i++) {
        lastSmoothData+=lastNFrames[i];
    }
    lastSmoothData/=lastNFrames.size();
}

