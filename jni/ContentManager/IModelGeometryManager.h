#ifndef IMODELGEOMETRYMANAGER_H
#define IMODELGEOMETRYMANAGER_H

#include "Handle.h"
#include "HandleManager.h"
#include "Graphics/ModelGeometry.h"


class ModelGeometryFactory {
public:
    ModelGeometry* Create() {return new ModelGeometry();}
};

struct tagModelGeometry{};

typedef Handle<ModelGeometry, ModelGeometryFactory, tagModelGeometry> ModelGeometryHandle;
typedef HandleManager<ModelGeometry, ModelGeometryFactory, tagModelGeometry> ModelGeometryHandleManager;

class IModelGeometryManager {
public:
    IModelGeometryManager(){}
    virtual ~IModelGeometryManager(){}

    virtual ModelGeometryHandle GetModelGeometry(const char* filename)=0;

    virtual void ReleaseModelGeometry(ModelGeometryHandle& handle)=0;
};

#endif // IMODELGEOMETRYMANAGER_H
