#ifndef RAY_H
#define RAY_H

#include "Vector3.h"
class Plane;

class Ray
{
public:
    Vector3 origin;
    Vector3 direction;


    Ray(void);
    Ray(const Vector3& orig, const Vector3& normal);

    bool Intersects(const Plane* plane, bool cullFaces, float*t, Vector3*hitPoint) const;
};

#endif // RAY_H
