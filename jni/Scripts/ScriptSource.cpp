#include "ScriptSource.h"

#include <FileIO/IFileIO.h>

ScriptSource::ScriptSource()
{
    source = " ";
    fileName = "No file loaded";
    sourceLenght = source.size();
}

ScriptSource::~ScriptSource() {
    Dispose();
}

bool ScriptSource::Load(const char *fileName) {
    IFileIO* fileIO = IFileIO::get();
    U32 size = fileIO->GetSize(fileName);

    char * buffer = new char[size+1];

    bool result = fileIO->Read(fileName, buffer, size);
    buffer[size] = '\0';



    if(result) {
        source = std::string(buffer);
        this->fileName = std::string(fileName);
        sourceLenght = source.size();
        delete[] buffer;
        buffer = NULL;
        return true;
    }
    else {
        delete[] buffer;
        buffer = NULL;
        return false;
    }
}

void ScriptSource::Dispose() {
    source.clear();
    fileName = "No file loaded";
    sourceLenght = source.size();
}

