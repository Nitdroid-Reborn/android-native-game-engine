#ifndef ICONTENTMANAGER_H
#define ICONTENTMANAGER_H
#include "ITextureManager.h"
#include "ISoundManager.h"
#include "IScriptSourceManager.h"

class IContentManager {
public:
    IContentManager(){}
    virtual ~IContentManager(){}

    virtual bool Initialize()=0;
    virtual bool Release()=0;

    ITextureManager* GetTextureManager() const {return textureManager;}
    ISoundManager* GetSoundManager() const {return soundManager;}
    ISciptSourceManager* GetScriptSourceManager() const {return scriptSourceManager;}

    static IContentManager* get(){return singleton;}

protected:
    static IContentManager* singleton;
    ITextureManager* textureManager;
    ISoundManager* soundManager;
    ISciptSourceManager* scriptSourceManager;
};

#endif // ICONTENTMANAGER_H
