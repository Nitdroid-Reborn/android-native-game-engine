#ifndef ITEXTUREMANAGER_H
#define ITEXTUREMANAGER_H

#include "Handle.h"
#include "HandleManager.h"
#include <Graphics/ITextureFactory.h>

struct tagTexture{};

typedef Handle<ITexture, ITextureFactory, tagTexture> TextureHandle;
typedef HandleManager<ITexture, ITextureFactory, tagTexture> TextureHandleManager;


//! Interface for texture manager
class ITextureManager {
public:
    ITextureManager(){}
    virtual ~ITextureManager(){}

    //! Gets texture
    /*!
      If texture is not loaded, loads it to memory, otherwise returns handle to it.
      \param filename path to texture file
      \returns handle to texture
    */
    virtual TextureHandle GetTexture(const char* filename)=0;
    //! Gets texture from raw data
    /*!
      Creates texture from raw data
      \param rawData raw data of texture
      \param info TextureInfo struct with informations about texture
      \returns handle to texture
      */
    virtual TextureHandle GetTexture(void* rawData, TextureInfo& info)=0;

    //! Releases texture
    /*!
      Releases reference to texture from handle, if reference count for this texture drops to 0 deletes it
      \param handle handle to texture
      */
    virtual void ReleaseTexture(TextureHandle& handle)=0;
};

#endif // ITEXTUREMANAGER_H
