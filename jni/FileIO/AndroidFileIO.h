#ifndef ANDROIDFILEIO_H
#define ANDROIDFILEIO_H
#include "IFileIO.h"
#include <android/asset_manager.h>
#include <stdio.h>

class AndroidFileIO : public IFileIO
{
public:
    explicit AndroidFileIO(AAssetManager* assetManager);
    ~AndroidFileIO();

    bool Initialize();
    bool Release();

    U32 GetFileSize(const char *path);
    U32 GetAssetSize(const char *path);

    virtual bool OpenFile(const char* path, const char* mode, EngineFileHandle& fileHandle);
    virtual void CloseFile(EngineFileHandle fileHandle);

    virtual bool ReadFromFile(EngineFileHandle fileHandle, void* buffer, U32 bufferSize);
    virtual bool ReadFromFile(EngineFileHandle fileHandle, void* buffer, U32 bufferSize, U32& bytesRead);
    virtual bool WriteToFile(EngineFileHandle fileHandle, void*buffer, U32 bufferSize);
    virtual bool WriteToFile(EngineFileHandle fileHandle, void*buffer, U32 bufferSize, U32& bytesWrite);

    virtual bool ReadAsset(const char* path, void* buffer, U32 bufferSize);
    virtual bool ReadAsset(const char* path, void* buffer, U32 bufferSize, U32& bytesRead);

    virtual bool ReadFile(const char* path, void* buffer, U32 bufferSize);
    virtual bool ReadFile(const char* path, void* buffer, U32 bufferSize, U32& bytesRead);

    virtual bool WriteFile(const char* path, void* buffer, U32 bufferSize);
    virtual bool WriteFile(const char* path, void* buffer, U32 bufferSize, U32& byteWrite);

private:
    AAssetManager* assetManager;
    FILE* fileHandles[256];
    U8 numHandles;
};

#endif // ANDROIDFILEIO_H
