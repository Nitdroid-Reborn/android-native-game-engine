#include "TextBox.h"
#include "SpriteBatcher.h"
#include <Graphics/IRenderer.h>
#include <ContentManager/IContentManager.h>

#define FONT_W 8
#define FONT_H 8
#define FONT_DRAW_W 12
#define FONT_DRAW_H 14
#define FONT_TEX_W 64
#define FONT_TEX_H 128
static unsigned int fontID = 0;

TextBox::TextBox() {

}

void TextBox::SetSize(int boxL, int boxR, int boxT, int boxB) {
    this->boxL = boxL;

    this->boxL = boxL;
    this->boxR = boxL + ((boxR - boxL) - ((boxR - boxL) % FONT_DRAW_W));
    this->boxT = boxT;
    this->boxB = boxT + ((boxB - boxT) - ((boxB - boxT) % FONT_DRAW_H));

    // Initialize the Buffers and Pointers
   // buffer[0] = '\0';
   // drawBufferPtr = &buffer[0];

    // Calculate the number of lines per Page
  //  pageLineCnt = (this->boxB - this->boxT) / FONT_DRAW_H;
   //z scrollCnt = 0;
    currX = this->boxL;
}


void TextBox::DrawStr(int x, int y, int maxFlag, int maxY, char* string) {
    int cursorX;
    int cursorY;
    char* cP = string;
    int index;
    int s, t;

    cursorX = x;
    cursorY = y;

    IRenderer*renderer= IRenderer::get();


    // Parse the String
    while (*cP != '\0') {
        // Visible characters
        if ( ' ' == *cP) {
                cursorX += FONT_DRAW_W;
        }
        else if ( ' ' <= *cP && *cP <= '~' ) {
            index = *cP - ' ';
            s = ( index % 8 ) * FONT_W;
            t = ( index / 8 ) * FONT_H;



            TextureRegion region((float)(s)/FONT_TEX_W,
                                 (float)(t+FONT_H + 1)/FONT_TEX_H,
                                 (float)(s+FONT_W)/FONT_TEX_W,
                                 (float)(t)/FONT_TEX_H);

            renderer->DrawSprite((F32)(cursorX + FONT_DRAW_W/2), (F32)(cursorY + FONT_DRAW_H),
                                 (F32)(FONT_DRAW_W), (F32)(FONT_DRAW_H), region, fontTex);

           cursorX += FONT_DRAW_W;
        }
        if ( *cP == '\n' ) {
                cursorX = x;
                cursorY += FONT_DRAW_H;
                if (maxFlag && cursorY + FONT_DRAW_H > maxY) break;
        }
        cP++;
    }

   // glEnd( );
}

void TextBox::DrawStr(int x, int y, char * str) {
    DrawStr(x, y, 0, 0, str);
}

void TextBox::FormatStrCat(char * str) {
        char buff[MAX_STRING];
        char buff2[MAX_STRING];
        char * cP;
        char * cP2;

        strcpy(buff, str);
        cP = buff;

        // Format String for caption
        // Parse the String
        while (*cP != '\0') {
                // Visible characters
                if ( ' ' <= *cP && *cP <= '~' ) {
                        currX += FONT_DRAW_W;
                }
                else if ( *cP == '\n' ) {
                        currX = boxL;
                }

                // Wrap
                if (currX >= boxR) {
                        cP++;
                        // Find the last space
                        while(cP > buff && *cP != ' ') cP--;
                        strcpy(buff2, cP);
                        *cP++ = '\n';
                        *cP = '\0';
                        // Eliminate Space
                        if (buff2[0] == ' ') cP2 = &buff2[1];
                        else cP2 = buff2;
                        strcat(cP, cP2);
                        currX = boxL;
                }
                else {
                        cP++;
                }
        }

        // Make sure it fits in the text buffer by removing
        // strings at the top of the buffer.
        cP = &buffer[0];
        while (strlen(cP) + strlen(buff) > MAX_STRING) {
                cP = strchr(cP, '\n');
                if( !cP ) {
                        break;
                }
                cP++;
        }
        // If there are strings that need to be removed. Do so.
        if (cP != &buffer[0]) {
                strcpy(buff2, cP);
                strcpy(buffer, buff2);
        }
        // Now there is space. So, concatinate.
        strcat(buffer, buff);
}

void TextBox::Printf(char* fmt, ... ) {
        va_list  vlist;
        char buff[MAX_STRING];

        // Get output string
        va_start(vlist, fmt);
        vsprintf(buff, fmt, vlist);

        FormatStrCat(buff);

        va_end(vlist);
}

void TextBox::Draw() {
        DrawStr(boxL, boxT, 1, boxB, buffer);
}

