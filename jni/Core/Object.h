#ifndef OBJECT_H
#define OBJECT_H
#include <Utils/Utils.h>

class Object
{
public:
    Object() : referenceCount(0){}
    virtual ~Object();

    U16 GetReferenceCount() const;
    void IncrementReferenceCount();
    void DecrementReferenceCount();

private:
    U16 referenceCount;
};

inline U16 Object::GetReferenceCount() const {
    return referenceCount;
}

inline void Object::IncrementReferenceCount() {
    referenceCount++;
}

inline void Object::DecrementReferenceCount() {
    referenceCount--;

    ASSERT(referenceCount>=0, "Reference count cannot be smaller than 0");
}

#endif // OBJECT_H
