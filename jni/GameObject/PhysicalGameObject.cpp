#include "PhysicalGameObject.h"

PhysicalGameObject::PhysicalGameObject(Hash _id, const Vector3 &_position, const Vector3 &_orientation, const Vector3 &_scale, string modelName, string shaderProgramName, bool transparent) :
    RenderableGameObject(_id, _position, _orientation, _scale, modelName, shaderProgramName, transparent), collider(0)
{
    collider = new Collider(modelHandle.Get());
}

PhysicalGameObject::~PhysicalGameObject() {
    if(collider)
        delete collider;
}

bool PhysicalGameObject::Collide(const Vector3 &point, Vector3& pos, Vector3& normal, Vector3& distance) {

    Matrix4x4 translation;
    Matrix4x4 rotation;
    Matrix4x4 scale;

    translation.SetTranslation(position);
    rotation.SetRotationEuler(orientation.x, orientation.y, orientation.z);
    scale.SetScale(this->scale);

    Matrix4x4 worldMatrix = translation*rotation*scale;
    Matrix4x4 invWorldMatrix = worldMatrix.GetInverse();

    Vector3 pointInModelSpace = invWorldMatrix*point;

    CollisionObject result;

    bool col = collider->Collide(pointInModelSpace, result);
    distance.y = result.distance;
    if(col) {
        pos = worldMatrix*result.position;
        Vector4 normal4 = Vector4(result.normal, 0);
        normal4 = worldMatrix*normal4;
        normal = result.normal;//Vector3(normal4.x, normal4.y, normal4.z);

    }
    return col;
}

