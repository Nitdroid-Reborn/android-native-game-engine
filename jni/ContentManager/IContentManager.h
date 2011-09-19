#ifndef ICONTENTMANAGER_H
#define ICONTENTMANAGER_H
#include "ITextureManager.h"
#include "ISoundManager.h"

class IContentManager {
public:
    IContentManager(){}
    virtual ~IContentManager(){}

    virtual bool Initialize()=0;
    virtual bool Release()=0;

    ITextureManager* GetTextureManager(){return textureManager;}
    ISoundManager* GetSoundManager(){return soundManager;}

    static IContentManager* get(){return singleton;}

protected:
    static IContentManager* singleton;
    ITextureManager* textureManager;
    ISoundManager* soundManager;
};

int IContentManagerGet(lua_State* l);


OOLUA_PROXY_CLASS(IContentManager)
    OOLUA_MEM_FUNC(ISoundManager*, GetSoundManager)
    OOLUA_MEM_FUNC(ITextureManager*, GetTextureManager)
    OOLUA_TYPEDEFS Abstract OOLUA_END_TYPES
OOLUA_CLASS_END
#endif // ICONTENTMANAGER_H
