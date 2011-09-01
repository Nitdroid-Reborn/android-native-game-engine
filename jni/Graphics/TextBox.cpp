#include "TextBox.h"
#include "SpriteBatcher.h"
#include <Graphics/IRenderer.h>

#define FONT_W 8
#define FONT_H 8
#define FONT_DRAW_W 8
#define FONT_DRAW_H 9
#define FONT_TEX_W 64
#define FONT_TEX_H 128
static unsigned int fontID = 0;

TextBox::TextBox(int boxL, int boxR, int boxT, int boxB) {
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

   /* if (!fontID) {
            // Create Texture
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glGenTextures(1, &fontID);
            glBindTexture(GL_TEXTURE_2D, fontID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_LUMINANCE_ALPHA,
                    FONT_TEX_W,
                    FONT_TEX_H,
                    0,
                    GL_LUMINANCE_ALPHA,
                    GL_UNSIGNED_BYTE,
                    (void *)fontData);
    }*/
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

            TextureRegion region(s, t,
                                 (float)(s+FONT_W)/FONT_TEX_W,
                                 (float)(t+FONT_H + 1)/FONT_TEX_H, 0);

            renderer->DrawSprite((F32)(cursorX + FONT_DRAW_W/2), (F32)(cursorY + FONT_DRAW_H),
                                 (F32)(FONT_DRAW_W), (F32)(FONT_DRAW_H), region);


         /*   glTexCoord2f((float)(s       )/FONT_TEX_W, (float)(t           )/FONT_TEX_H);
            glVertex3s(cursorX            , cursorY            , 0);


            glTexCoord2f((float)(s       )/FONT_TEX_W, (float)(t+FONT_H + 1)/FONT_TEX_H);
            glVertex3s(cursorX            , cursorY+FONT_DRAW_H, 0);


            glTexCoord2f((float)(s+FONT_W)/FONT_TEX_W, (float)(t+FONT_H + 1)/FONT_TEX_H);
            glVertex3s(cursorX+FONT_DRAW_W, cursorY+FONT_DRAW_H, 0);


            glTexCoord2f((float)(s+FONT_W)/FONT_TEX_W, (float)(t           )/FONT_TEX_H);
            glVertex3s(cursorX+FONT_DRAW_W, cursorY            , 0);*/

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
