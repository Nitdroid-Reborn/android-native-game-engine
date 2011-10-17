#ifndef SHADERSOURCE_H
#define SHADERSOURCE_H

#include <string>

class ShaderSource
{
public:
    ShaderSource();
    ~ShaderSource();

    bool Load(const char* file);
    void Dispose();
    inline const char* GetFileName() const {return fileName.c_str();}
    inline const char* GetSource() const {return source.c_str();}

private:
    std::string source;
    std::string fileName;
};

#endif // SHADERSOURCE_H
