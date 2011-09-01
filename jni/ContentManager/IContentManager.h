#ifndef ICONTENTMANAGER_H
#define ICONTENTMANAGER_H
#include "ITextureManager.h"

class IContentManager {
public:
    IContentManager(){}
    virtual ~IContentManager(){}

    virtual bool Initialize()=0;
    virtual bool Release()=0;

    ITextureManager* GetTextureManager(){return textureManager;}

    static IContentManager* get(){return singleton;}

protected:
    static IContentManager* singleton;
    ITextureManager* textureManager;
};

#endif // ICONTENTMANAGER_H
