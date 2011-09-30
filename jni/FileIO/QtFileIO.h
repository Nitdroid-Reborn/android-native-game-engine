#ifndef QTFILEIO_H
#define QTFILEIO_H
#include "IFileIO.h"

class QtFileIO : public IFileIO
{
public:
    explicit QtFileIO();
    ~QtFileIO();

    bool Initialize();
    bool Release();

    U32 GetFileSize(const char *path);
    U32 GetAssetSize(const char *path);

    bool OpenFile(const char* path, const char* mode, EngineFileHandle& fileHandle);
    void CloseFile(EngineFileHandle fileHandle);

    bool ReadFromFile(EngineFileHandle fileHandle, void* buffer, U32 bufferSize);
    bool ReadFromFile(EngineFileHandle fileHandle, void* buffer, U32 bufferSize, U32& bytesRead);
    bool WriteToFile(EngineFileHandle fileHandle, void*buffer, U32 bufferSize);
    bool WriteToFile(EngineFileHandle fileHandle, void*buffer, U32 bufferSize, U32& bytesWrite);

    bool ReadAsset(const char* path, void* buffer, U32 bufferSize);
    bool ReadAsset(const char* path, void* buffer, U32 bufferSize, U32& bytesRead);

    bool ReadFile(const char* path, void* buffer, U32 bufferSize);
    bool ReadFile(const char* path, void* buffer, U32 bufferSize, U32& bytesRead);

    bool WriteFile(const char* path, void* buffer, U32 bufferSize);
    bool WriteFile(const char* path, void* buffer, U32 bufferSize, U32& byteWrite);

    bool Read(const char* path, void*buffer, U32 bufferSize);
    bool Read(const char* path, void* buffer, U32 bufferSize, U32& bytesRead);
    U32 GetSize(const char* path);

private:
    FILE* fileHandles[256];
    U8 numHandles;
};
#endif // QTFILEIO_H
