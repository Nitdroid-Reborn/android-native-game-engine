#include <Math/Ray.h>

#include <Math/Plane.h>

Ray::Ray(void)
{
    origin = Vector3(0,0,0);
    direction = Vector3(0,0,1);
}

Ray::Ray(const Vector3 &orig, const Vector3 &dir) : origin(orig), direction(dir) {

}

bool Ray::Intersects(const Plane *plane,
                     bool cullFaces,
                     float *hitDistance,
                     Vector3 *hitPoint) const {
    float planeDot = plane->normal.DotProduct(direction);

    // ray parallel to the plane
    if(fabs(planeDot) < 0.00001f)
        return false;


    if(cullFaces && (planeDot > 0.0f))
        return false;

    float dist = -((plane->normal.DotProduct(origin)) + plane->distance);

    float _t = dist / planeDot;

    // intersection before ray origin
    if (_t < 0.0f)
        return false;

    if (hitPoint) {
        (*hitPoint) = origin + (direction * _t);
    }
    if (hitDistance)
        (*hitDistance) = _t;

    return true;
}
