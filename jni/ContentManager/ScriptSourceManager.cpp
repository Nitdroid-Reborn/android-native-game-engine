#include "ScriptSourceManager.h"

ScriptSourceManager::ScriptSourceManager() : ISciptSourceManager() {
    ScriptSourceHandle::manager = &handleManager;
    handleManager.SetDataFactory(new ScriptSourceFactory());
}

ScriptSourceHandle ScriptSourceManager::GetScriptSource(const char *filename) {
    if(loadedScripts.count(filename)!=0)
        return loadedScripts[filename];

    ScriptSourceHandle handle;
    ScriptSource* scriptSrc = handleManager.Acquire(handle);
    scriptSrc->Load(filename);
    loadedScripts[filename] = handle;

    return handle;
}

void ScriptSourceManager::ReleaseScriptSource(ScriptSourceHandle &handle) {
    handleManager.Release(handle);
}
