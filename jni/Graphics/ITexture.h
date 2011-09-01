#ifndef ITEXTURE_H
#define ITEXTURE_H
#include <Utils/EngineTypes.h>
#include <Core/Object.h>


struct TextureInfo {
    enum TextureFormat {
        RGB8      = 0,
        RGBA8     = 0
    };

    U16 width;
    U16 height;
    U32 minFilter;
    U32 magFilter;
    TextureFormat format;
};

class ITexture : public Object {
public:
    ITexture(): Object() {}
    virtual ~ITexture(){}
    virtual bool Load(const char* filename)=0;
    virtual bool Reload()=0;
    virtual void SetFilters(U32 minFilter, U32 magFilter)=0;
    virtual void Bind()=0;
    virtual void Dispose()=0;
    virtual const TextureInfo GetInfo(){return (const TextureInfo)textureInfo;}

protected:
    const char *filename;
    TextureInfo textureInfo;
};

#endif // ITEXTURE_H
