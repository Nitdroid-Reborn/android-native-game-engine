#include "Object.h"

Object::~Object() {
    ASSERT(referenceCount == 0, "Freeing object with existing references to it");
}
