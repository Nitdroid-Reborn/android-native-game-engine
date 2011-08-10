#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vector3.h"
#include "Matrix4x4.h"

class BoundingBox
{
public:
    Vector3 vertices[8];
    BoundingBox(const Vector3& min, const Vector3& max);

    void applyTransformation(const Matrix4x4& transformation);
};

#endif // BOUNDINGBOX_H
