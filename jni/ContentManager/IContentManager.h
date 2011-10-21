#ifndef ICONTENTMANAGER_H
#define ICONTENTMANAGER_H
#include "ITextureManager.h"
#include "ISoundManager.h"
#include "IScriptSourceManager.h"
#include "IShaderManager.h"
#include "IShaderProgramManager.h"
#include "IModelGeometryManager.h"

//! Interface for game content manager
class IContentManager {
public:
    IContentManager(){}
    virtual ~IContentManager(){}

    //! Initializes subsystem
    virtual bool Initialize()=0;

    //! Releases subsystem
    virtual bool Release()=0;

    //! Returns TextureManager
    ITextureManager* GetTextureManager() const {return textureManager;}

    //! Returns SoundManager
    ISoundManager* GetSoundManager() const {return soundManager;}

    //! Returns ScriptSourceManager
    ISciptSourceManager* GetScriptSourceManager() const {return scriptSourceManager;}

    //! Returns ScriptSourceManager
    IShaderManager* GetShaderManager() const {return shaderManager;}

    IShaderProgramManager* GetShaderProgramManager() const {return shaderProgramManager;}

    IModelGeometryManager* GetModelGeometryManager() const {return modelGeometryManager;}

    //! Returns instance of content manager
    static IContentManager* get(){return singleton;}

protected:
    static IContentManager* singleton;
    ITextureManager* textureManager;
    ISoundManager* soundManager;
    ISciptSourceManager* scriptSourceManager;
    IShaderManager* shaderManager;
    IShaderProgramManager* shaderProgramManager;
    IModelGeometryManager* modelGeometryManager;
};

#endif // ICONTENTMANAGER_H
