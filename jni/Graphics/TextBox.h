#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <ContentManager/ITextureManager.h>

#define MAX_STRING 4096

class TextBox
{
public:
    TextBox();
    void SetSize(int boxL, int boxR, int boxT, int boxB);
    ~TextBox(){}

private:
    // Bounding Box
    int boxL;
    int boxR;
    int boxT;
    int boxB;

    // Text Buffers
    char buffer[MAX_STRING];
    char drawBufferPtr[MAX_STRING];

    // Info
    int pageLineCnt;
    int scrollCnt;
    int currX;
    int currY;

public:
    void DrawStr(int x, int y, char* string);
    void Printf(char* fmt, ...);
    void FormatStrCat(char * str);
    void Draw();
    TextureHandle fontTex;


private:
    void DrawStr(int x, int y, int maxFlag, int maxY, char* string);
};

#endif // TEXTBOX_H
