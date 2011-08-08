#include "AndroidFileIO.h"
#include <sys/stat.h>
#include <assert.h>
#include "Utils.h"

FileIO* FileIO::singleton=NULL;

AndroidFileIO::AndroidFileIO() : FileIO()
{

}

AndroidFileIO::~AndroidFileIO() {

}

bool AndroidFileIO::Initialize(void *assetManager) {
    this->assetManager = (AAssetManager*)assetManager;
    if(singleton!=NULL) {
        LOGE("FileIO system already initialized");
    }
    assert(singleton==NULL);
    singleton = this;
    return true;
}

bool AndroidFileIO::Release() {
    singleton = NULL;
    return true;
}


U32 AndroidFileIO::GetAssetSize(const char *path) {
    AAsset* asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);

    if(asset == NULL)
        return 0;


    int len = AAsset_getLength(asset);
    AAsset_close(asset);
    return len;
}

U32 AndroidFileIO::GetFileSize(const char *path) {
    struct stat st;
    size_t fileSize=0;
    if(stat(path,&st) == 0 ){
         fileSize = st.st_size;
    }
    return fileSize;
}


bool AndroidFileIO::ReadAsset(const char *path, U8* buffer, U32 bufferSize, U32& bytesRead) {
    AAsset* asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);

    if(asset == NULL)
        return false;

    bytesRead = AAsset_read(asset, buffer, bufferSize);

    AAsset_close(asset);
}


bool AndroidFileIO::ReadFile(const char *path, U8 *buffer, U32 bufferSize, U32 &bytesRead) {
    FILE* pf = fopen(path, "r");
    if(pf == NULL)
        return false;


    size_t read = fread(buffer, 1, bufferSize, pf);

    int error = ferror(pf);
    fclose(pf);

    if(0 == error) {
        bytesRead = read;
        return true;
    }
    return false;
}


bool AndroidFileIO::WriteFile(const char *path, U8 *buffer, U32 bufferSize, U32 &byteWrite) {
    FILE* pf = fopen(path, "w");
    if(pf == NULL)
        return false;


    size_t write = fwrite(buffer, 1, bufferSize, pf);

    int error = ferror(pf);
    fclose(pf);

    if(0 == error) {
        byteWrite = write;
        return true;
    }
    return false;
}


