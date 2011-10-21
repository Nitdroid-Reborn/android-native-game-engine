#ifndef ITEXTURE_H
#define ITEXTURE_H
#include <Utils/EngineTypes.h>
#include <Core/Object.h>
//#include <Scripts/oolua/oolua.h>

struct TextureInfo {
    enum TextureFormat {
        RGB8      = 0,
        RGBA8     = 1,
        LUMINANCE_ALPHA =2
    };

    U16 width;
    U16 height;
    U32 minFilter;
    U32 magFilter;
    U16 pixelAlignment;
    TextureFormat format;
};

class ITexture : public Object {
public:
    ITexture(): Object() {}
    virtual ~ITexture(){}
    virtual bool Load(const char* filename)=0;
    virtual bool Load(void* rawData, TextureInfo& info)=0;
    virtual bool Reload()=0;
    virtual void SetFilters(U32 minFilter, U32 magFilter)=0;
    virtual void Bind()=0;
    virtual void Unbind()=0;
    virtual void Dispose()=0;
    inline virtual const TextureInfo GetInfo(){return (const TextureInfo)textureInfo;}

protected:
    const char *filename;
    TextureInfo textureInfo;
};


#endif // ITEXTURE_H
