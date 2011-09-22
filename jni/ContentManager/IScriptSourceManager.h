#ifndef ISCRIPTSOURCEMANAGER_H
#define ISCRIPTSOURCEMANAGER_H

#include "Handle.h"
#include "HandleManager.h"
#include <Scripts/ScriptSourceFactory.h>

struct tagScript{};

typedef Handle<ScriptSource, ScriptSourceFactory, tagScript> ScriptSourceHandle;
typedef HandleManager<ScriptSource, ScriptSourceFactory, tagScript> ScriptSourceHandleManager;

class ISciptSourceManager {
public:
    ISciptSourceManager(){}
    virtual ~ISciptSourceManager(){}

    virtual ScriptSourceHandle GetScriptSource(const char* filename)=0;
    virtual void ReleaseScriptSource(ScriptSourceHandle& handle)=0;
};

#endif // ISCRIPTSOURCEMANAGER_H
