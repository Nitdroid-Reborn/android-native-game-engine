#ifndef QTCONTENTMANAGER_H
#define QTCONTENTMANAGER_H

#include "IContentManager.h"

class QtContentManager : public IContentManager
{
public:
    QtContentManager();
    ~QtContentManager();

    bool Initialize();
    bool Release();
};

#endif // QTCONTENTMANAGER_H
