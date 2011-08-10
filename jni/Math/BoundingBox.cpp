#include <Math/MathLib.h>
#include <Math/BoundingBox.h>

BoundingBox::BoundingBox(const Vector3 &min, const Vector3 &max)
{
    Vector3 sizes = max - min;

    vertices[0] = min;
    vertices[1] = Vector3(min.x+sizes.x, min.y, min.z);
    vertices[2] = Vector3(min.x+sizes.x, min.y, min.z+sizes.z);
    vertices[3] = Vector3(min.x, min.y, min.z+sizes.z);

    vertices[4] = Vector3(min.x, min.y+sizes.y, min.z);
    vertices[5] = Vector3(min.x+sizes.x, min.y+sizes.y, min.z);
    vertices[6] = Vector3(min.x+sizes.x, min.y+sizes.y, min.z+sizes.z);
    vertices[7] = Vector3(min.x, min.y+sizes.y, min.z+sizes.z);
}

void BoundingBox::applyTransformation(const Matrix4x4 &transformation) {
    for(int i=0;i<8;i++) {
        vertices[i] = transformation*vertices[i];
    }
}
