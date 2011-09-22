#ifndef SCRIPTSOURCEFACTORY_H
#define SCRIPTSOURCEFACTORY_H

#include <Scripts/ScriptSource.h>

class ScriptSourceFactory
{
public:
    ScriptSourceFactory(){}

    ScriptSource* Create();
};

#endif // SCRIPTSOURCEFACTORY_H
