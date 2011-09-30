#include "QtFileIO.h"
#include <sys/stat.h>
IFileIO* IFileIO::singleton=NULL;


QtFileIO::QtFileIO() : IFileIO() {

}

QtFileIO::~QtFileIO() {

}

bool QtFileIO::Initialize() {
    ASSERT(!singleton, "FileIO system already initialized");

    singleton = this;

    for(int i=0;i<256;i++)
        fileHandles[i]=NULL;

    numHandles=0;

    Logger::Log(1, "Qt FileIO system initialized");
    return true;
}

bool QtFileIO::Release() {
    if(numHandles!=0);
    for(int i=0;i<256;i++) {
        if(fileHandles[i]!=NULL) {
            fclose(fileHandles[i]);
            fileHandles[i] = NULL;
        }
    }
    singleton = NULL;
    Logger::Log(1, "Qt FileIO system released");
    return true;
}

bool QtFileIO::OpenFile(const char *path, const char *mode, EngineFileHandle &fileHandle) {
    if(numHandles>=256) return false;

    for(int i=0;i<256;i++) {
        if(fileHandles[i]==NULL) {
            fileHandle = i;
            numHandles++;
            break;
        }
    }

    fileHandles[fileHandle] = fopen(path, mode);
    if(fileHandles[fileHandle]) {
        return 1;
    }

    fileHandles[fileHandle] = NULL;
    numHandles--;
    return -1;
}

void QtFileIO::CloseFile(EngineFileHandle fileHandle) {
    if(fileHandle>=0 && fileHandle<256) {
        fclose(fileHandles[fileHandle]);
        fileHandles[fileHandle]=NULL;
        numHandles--;
    }
}

bool QtFileIO::ReadFromFile(EngineFileHandle fileHandle, void *buffer, U32 bufferSize) {
    if(fileHandle>=0 && fileHandle<256) {
        size_t read = fread(buffer, 1, bufferSize, fileHandles[fileHandle]);

        int error = ferror(fileHandles[fileHandle]);
        if(0 == error) {
            return true;
        }
        return false;
    }
    return false;
}

bool QtFileIO::ReadFromFile(EngineFileHandle fileHandle, void *buffer, U32 bufferSize, U32& bytesRead) {
    if(fileHandle>=0 && fileHandle<256) {
        size_t read = fread(buffer, 1, bufferSize, fileHandles[fileHandle]);

        int error = ferror(fileHandles[fileHandle]);
        if(0 == error) {
            bytesRead = read;
            return true;
        }
        return false;
    }
    return false;
}


bool QtFileIO::WriteToFile(EngineFileHandle fileHandle, void *buffer, U32 bufferSize) {
    if(fileHandle>=0 && fileHandle<256) {
        size_t read = fwrite(buffer, 1, bufferSize, fileHandles[fileHandle]);

        int error = ferror(fileHandles[fileHandle]);
        if(0 == error) {
            return true;
        }
        return false;
    }
    return false;
}

bool QtFileIO::WriteToFile(EngineFileHandle fileHandle, void *buffer, U32 bufferSize, U32& bytesWrite) {
    if(fileHandle>=0 && fileHandle<256) {
        size_t read = fread(buffer, 1, bufferSize, fileHandles[fileHandle]);

        int error = ferror(fileHandles[fileHandle]);
        if(0 == error) {
            bytesWrite = read;
            return true;
        }
        return false;
    }
    return false;
}

U32 QtFileIO::GetAssetSize(const char *path) {
    /*AAsset* asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);

    if(asset == NULL)
        return 0;


    int len = AAsset_getLength(asset);
    AAsset_close(asset);
    return len;*/
    return 0;
}

U32 QtFileIO::GetFileSize(const char *path) {
    struct stat st;
    size_t fileSize=0;
    if(stat(path,&st) == 0 ){
         fileSize = st.st_size;
    }
    return fileSize;
}


bool QtFileIO::ReadAsset(const char *path, void* buffer, U32 bufferSize) {
    /*AAsset* asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);

    if(asset == NULL)
        return false;

    AAsset_read(asset, buffer, bufferSize);

    AAsset_close(asset);
*/
    return false;

}

bool QtFileIO::ReadAsset(const char *path, void* buffer, U32 bufferSize, U32& bytesRead) {
    /*AAsset* asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);

    if(asset == NULL)
        return false;

    bytesRead = AAsset_read(asset, buffer, bufferSize);

    AAsset_close(asset);
*/
    bytesRead = 0;
    return false;
}


bool QtFileIO::ReadFile(const char *path, void *buffer, U32 bufferSize) {
    FILE* pf = fopen(path, "r");
    if(pf == NULL)
        return false;

    size_t read = fread(buffer, 1, bufferSize, pf);

    int error = ferror(pf);
    fclose(pf);

    if(0 == error) {
        return true;
    }
    return false;
}

bool QtFileIO::ReadFile(const char *path, void *buffer, U32 bufferSize, U32 &bytesRead) {
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


bool QtFileIO::WriteFile(const char *path, void *buffer, U32 bufferSize) {
    FILE* pf = fopen(path, "w");
    if(pf == NULL)
        return false;


    size_t write = fwrite(buffer, 1, bufferSize, pf);

    int error = ferror(pf);
    fclose(pf);

    if(0 == error) {
        return true;
    }
    return false;
}

bool QtFileIO::WriteFile(const char *path, void *buffer, U32 bufferSize, U32 &byteWrite) {
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



U32 QtFileIO::GetSize(const char *path) {
    if(path[0] == ':') {
        const char* assetPath = &path[1];
        return GetAssetSize(assetPath);
    }
    else {
        return GetFileSize(path);
    }
}

bool QtFileIO::Read(const char *path, void *buffer, U32 bufferSize) {
    if(path[0] == ':') {
        const char* assetPath = &path[1];
        return ReadAsset(assetPath, buffer, bufferSize);
    }
    else {
        return ReadFile(path, buffer, bufferSize);
    }
}

bool QtFileIO::Read(const char *path, void *buffer, U32 bufferSize, U32 &bytesRead) {
    if(path[0] == ':') {
        const char* assetPath = &path[1];
        return ReadAsset(assetPath, buffer, bufferSize, bytesRead);
    }
    else {
        return ReadFile(path, buffer, bufferSize, bytesRead);
    }
}
