#include "TextBuffer.h"
#include <string.h>

TextBuffer::TextBuffer()
{
    currentPos=0;
    memset(buffer, '\0', 4096);
}

void TextBuffer::Clear() {
    currentPos=0;
    memset(buffer, '\0', 4096);
}

void TextBuffer::Print(const char *txt) {
    while(*txt!='\0') {
        if(currentPos<4095)
            buffer[currentPos++] = *txt;
        txt++;
    }
}

char* TextBuffer::Get() {
    return &buffer[0];
}
