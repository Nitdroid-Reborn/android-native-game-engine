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

    struct TextBatch {
        std::string text;
        U16 x;
        U16 y;
    };

    vector<TextBatch> texts;
    vector<TextBatch> oldTexts;

    // Info
    int pageLineCnt;
    int scrollCnt;
    int currX;
    int currY;

public:
    void DrawStr(int x, int y, char* string);
    void Draw();
    void SwapTextBuffer();
    Font* font;
};

#endif // TEXTBOX_H
