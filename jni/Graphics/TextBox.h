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
    Font* font;
};

#endif // TEXTBOX_H
