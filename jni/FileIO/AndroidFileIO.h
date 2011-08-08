#ifndef ANDROIDFILEIO_H
#define ANDROIDFILEIO_H
#include "FileIO.h"
#include <android/asset_manager.h>


class AndroidFileIO : public FileIO
{
public:
    explicit AndroidFileIO();
    ~AndroidFileIO();


    bool Initialize(void* assetManager);
    bool Release();

    U32 GetFileSize(const char *path);
    U32 GetAssetSize(const char *path);
    bool ReadAsset(const char* path, U8* buffer, U32 bufferSize, U32& bytesRead);
    bool ReadFile(const char* path, U8* buffer, U32 bufferSize, U32& bytesRead);
    bool WriteFile(const char* path, U8* buffer, U32 bufferSize, U32& byteWrite);

private:
    AAssetManager* assetManager;
};

#endif // ANDROIDFILEIO_H
