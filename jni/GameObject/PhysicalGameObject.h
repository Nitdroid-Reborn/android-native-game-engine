#ifndef PHYSICALGAMEOBJECT_H
#define PHYSICALGAMEOBJECT_H
#include "RenderableGameObject.h"
#include "ContentManager/IContentManager.h"
#include "Physics/Collider.h"

class PhysicalGameObject : public RenderableGameObject
{
public:
    PhysicalGameObject(Hash _id, const Vector3& _position, const Vector3& _orientation,
                       const Vector3& _scale, std::string modelName, std::string shaderProgramName,
                       bool transparent=false);

    ~PhysicalGameObject();

    bool Collide(const Vector3& point, Vector3 &pos, Vector3 &normal);

protected:
    Collider* collider;
};

#endif // PHYSICALGAMEOBJECT_H
