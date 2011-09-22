#ifndef SCRIPTSOURCE_H
#define SCRIPTSOURCE_H

#include <string>
#include <Utils/EngineTypes.h>

class ScriptSource
{
public:
    ScriptSource();
    ~ScriptSource();

    bool Load(const char* fileName);
    void Dispose();
    inline const char* GetFileName() const {return fileName.c_str();}
    inline const char* GetSource() const {return source.c_str();}
    inline U32 GetSourceLength() const {return sourceLenght;}

private:
    std::string source;
    std::string fileName;
    U32 sourceLenght;
};

#endif // SCRIPTSOURCE_H
