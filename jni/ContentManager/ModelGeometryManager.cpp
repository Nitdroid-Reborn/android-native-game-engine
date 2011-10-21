#include "ModelGeometryManager.h"

ModelGeometryManager::ModelGeometryManager() : IModelGeometryManager() {
    ModelGeometryHandle::manager = &handleManager;
    handleManager.SetDataFactory(new ModelGeometryFactory());

}

ModelGeometryManager::~ModelGeometryManager() {
    loadedModels.clear();
}

ModelGeometryHandle ModelGeometryManager::GetModelGeometry(const char *filename) {
    if(loadedModels.count(filename)!=0)
        return loadedModels[filename];

    ModelGeometryHandle handle;
    ModelGeometry* geometry = handleManager.Acquire(handle);

    geometry->Load(filename);

    loadedModels[filename] = handle;
    return handle;
}

void ModelGeometryManager::ReleaseModelGeometry(ModelGeometryHandle &handle) {
    handleManager.Release(handle);

}
