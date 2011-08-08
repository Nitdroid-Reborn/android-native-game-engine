#ifndef FILEIO_H
#define FILEIO_H
#include "EngineTypes.h"
#include <vector>
#include <string>
#include "Utils.h"

using namespace std;

class AndroidFileIO;


class FileIO
{
public:
    FileIO() {
        if(singleton!=NULL) {
            LOGE("FileIO system already created. There can be only one FileIO system.");
        }
        assert(singleton==NULL);
    }
    ~FileIO(){}




    virtual bool Initialize(void* assetManager)=0;
    virtual bool Release() = 0;

    virtual U32 GetAssetSize(const char* path)=0;
    virtual U32 GetFileSize(const char* path)=0;

    virtual bool ReadAsset(const char* path, U8* buffer, U32 bufferSize, U32& bytesRead)=0;
    virtual bool ReadFile(const char* path, U8* buffer, U32 bufferSize, U32& bytesRead)=0;
    virtual bool WriteFile(const char* path, U8* buffer, U32 bufferSize, U32& byteWrite)=0;

    static FileIO* get() {
        return singleton;
    }

protected:
    static FileIO* singleton;
};

#endif // FILEIO_H
