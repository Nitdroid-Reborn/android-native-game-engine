#ifndef ISCRIPTSOURCEMANAGER_H
#define ISCRIPTSOURCEMANAGER_H

#include "Handle.h"
#include "HandleManager.h"
#include <Scripts/ScriptSourceFactory.h>

struct tagScript{};

typedef Handle<ScriptSource, ScriptSourceFactory, tagScript> ScriptSourceHandle;
typedef HandleManager<ScriptSource, ScriptSourceFactory, tagScript> ScriptSourceHandleManager;


//! Interface for script source manager
class ISciptSourceManager {
public:
    ISciptSourceManager(){}
    virtual ~ISciptSourceManager(){}

    //! Gets script source
    /*!
      If source is not loaded, loads it to memory, otherwise returns handle to it.
      \param filename path to script file
      \returns handle to script source
    */
    virtual ScriptSourceHandle GetScriptSource(const char* filename)=0;

    //! Releases script source
    /*!
      Releases reference to script source from handle, if reference count for this source drops to 0 deletes it
      \param handle handle to script source
      */
    virtual void ReleaseScriptSource(ScriptSourceHandle& handle)=0;
};

#endif // ISCRIPTSOURCEMANAGER_H
