#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <Utils/Hash.h>
#include <Math/MathLib.h>

enum GameObjectType {
    Renderable=0,
    Logic
};

class GameObject {
public:
    GameObject(Hash _id, const Vector3& _position, const Vector3& _orientation,
               const Vector3& _scale) : id(_id), position(_position), orientation(_orientation), scale(_scale) {
    }
    ~GameObject(){}


    Hash GetId() const {return id;}
    Vector3 GetPosition() const {return position;}
    Vector3 GetOrientation() const {return orientation;}
    Vector3 GetScale() const {return scale;}

    void SetPosition(const Vector3& position) {this->position = position;}
    void SetOrientation(const Vector3& orientation) {this->orientation = orientation;}
    void SetScale(const Vector3& scale) {this->scale = scale;}

    static GameObjectType GetType() {return type;}

    virtual void Update(float dt)=0;

protected:
    Hash id;
    Vector3 position;
    Vector3 orientation;
    Vector3 scale;
    static GameObjectType type;
};

#endif // GAMEOBJECT_H
