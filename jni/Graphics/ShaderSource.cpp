#include "ShaderSource.h"
#include <FileIO/IFileIO.h>

ShaderSource::ShaderSource()
{
    source = " ";
    fileName = "No file loaded";
}

ShaderSource::~ShaderSource() {
    Dispose();
}

bool ShaderSource::Load(const char *fileName) {
    IFileIO* fileIO = IFileIO::get();
    U32 size = fileIO->GetSize(fileName);

    char* buffer = new char[size+1];
    bool result = fileIO->Read(fileName, buffer, size);
    buffer[size]='\0';

    if(result) {
        source = std::string(buffer);
        this->fileName = std::string(fileName);
        delete [] buffer;
        buffer = NULL;

        return true;
    }
    else {
        delete[] buffer;
        buffer=NULL;
        return false;
    }


}

void ShaderSource::Dispose() {
    source.clear();
    fileName = "No file loaded";
}

