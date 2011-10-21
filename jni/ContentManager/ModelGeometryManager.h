#ifndef MODELGEOMETRYMANAGER_H
#define MODELGEOMETRYMANAGER_H
#include "IModelGeometryManager.h"
#include <map>
#include <string>

class ModelGeometryManager : public IModelGeometryManager
{
    ModelGeometryHandleManager handleManager;
    std::map<std::string, ModelGeometryHandle> loadedModels;

public:
    ModelGeometryManager();
    ~ModelGeometryManager();

    ModelGeometryHandle GetModelGeometry(const char *filename);
    void ReleaseModelGeometry(ModelGeometryHandle &handle);
};

#endif // MODELGEOMETRYMANAGER_H
