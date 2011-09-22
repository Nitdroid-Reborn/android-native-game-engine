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

    char * buffer = new char[size];

    Logger::Log("loading script");
    bool result = fileIO->Read(fileName, buffer, size);



    if(result) {
        source = std::string(buffer);
        this->fileName = std::string(fileName);
        sourceLenght = source.size();
        delete[] buffer;
        return true;
    }
    else {
        delete[] buffer;
        return false;
    }
}

void ScriptSource::Dispose() {
    source.clear();
    fileName = "No file loaded";
    sourceLenght = source.size();
}

