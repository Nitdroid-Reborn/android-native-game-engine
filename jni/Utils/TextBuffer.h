#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H
#include <Utils/EngineTypes.h>

class TextBuffer
{
    char buffer[4096];
    U16 currentPos;
public:
    TextBuffer();
    void Print(const char*txt);
    void Clear();
    char* Get();
};

#endif // TEXTBUFFER_H
