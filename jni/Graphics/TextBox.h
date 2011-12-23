#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <ContentManager/ITextureManager.h>
#include "Font.h"

#define MAX_STRING 4096




class TextBox
{
public:
    TextBox();
    ~TextBox(){}

private:
    // Info
    int pageLineCnt;
    int scrollCnt;
    int currX;
    int currY;

public:
    void DrawStr(int x, int y, char* string);
    void DrawStr(int x, int y, char* string, U8 r, U8 g, U8 b);
    Font2* font;
};

#endif // TEXTBOX_H
