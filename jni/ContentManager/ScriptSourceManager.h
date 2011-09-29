#ifndef SCRIPTSOURCEMANAGER_H
#define SCRIPTSOURCEMANAGER_H
#include "IScriptSourceManager.h"
#include <map>
#include <string>

//! Implementation of script source manager
class ScriptSourceManager : public ISciptSourceManager
{
    ScriptSourceHandleManager handleManager;
    std::map<std::string, ScriptSourceHandle> loadedScripts;

public:
    ScriptSourceManager();
    ~ScriptSourceManager() {

    }

    ScriptSourceHandle GetScriptSource(const char *filename);
    void ReleaseScriptSource(ScriptSourceHandle &handle);
};

#endif // SCRIPTSOURCEMANAGER_H
