#ifndef TEXTBOX_H
#define TEXTBOX_H

#define MAX_STRING 4096

class TextBox
{
public:
    TextBox(int boxL, int boxR, int boxT, int boxB);
    ~TextBox();

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


private:
    void DrawStr(int x, int y, int maxFlag, int maxY, char* string);
};

#endif // TEXTBOX_H
