#include "TextBox.h"
#include "SpriteBatcher.h"
#include <Graphics/IRenderer.h>
#include <ContentManager/IContentManager.h>
#include <GLES/gl.h>

/*#define FONT_W 8
#define FONT_H 8
#define FONT_DRAW_W 12
#define FONT_DRAW_H 14
#define FONT_TEX_W 64
#define FONT_TEX_H 128*/
static unsigned int fontID = 0;

TextBox::TextBox() {

}


void TextBox::DrawStr(int x, int y, char *string) {
    TextBatch t;
    t.x = x;
    t.y = y;
    t.text = std::string(string);

    texts.push_back(t);
}


void TextBox::SwapTextBuffer() {
    oldTexts = texts;
    texts.clear();
}

void TextBox::Draw() {
    vector<TextBatch>::iterator it = oldTexts.begin();

    int cursorX;
    int cursorY;
    const char* cP;

    IRenderer*renderer= IRenderer::get();

    for(it; it!=oldTexts.end(); ++it) {
        cursorX = (*it).x;
        cursorY = (*it).y;
        cP = (const char*)(*it).text.c_str();

        while (*cP != '\0') {
            // Visible characters
            if ( 32 <= *cP && *cP<=126) {

                char ch = *cP;

                TextureRegion region(font->glyphs[ch].u1,
                                     font->glyphs[ch].v1,
                                     font->glyphs[ch].u2,
                                     font->glyphs[ch].v2);


                renderer->DrawSprite((F32)(cursorX + font->glyphs[ch].left + font->glyphs[ch].w/2), (F32)(cursorY + (font->glyphs[ch].top - font->glyphs[ch].h) + font->glyphs[ch].h/2),
                                     PROFILER_LAYER,
                                     (F32)(font->glyphs[ch].w), (F32)(font->glyphs[ch].h), region, font->fontTexture);

                /*

                index = *cP - ' ';
                s = ( index % 8 ) * FONT_W;
                t = ( index / 8 ) * FONT_H;



                TextureRegion region((float)(s)/FONT_TEX_W,
                                     (float)(t+FONT_H + 1)/FONT_TEX_H,
                                     (float)(s+FONT_W)/FONT_TEX_W,
                                     (float)(t)/FONT_TEX_H);

                renderer->DrawSprite((F32)(cursorX + FONT_DRAW_W/2), (F32)(cursorY + FONT_DRAW_H),
                                     PROFILER_LAYER,
                                     (F32)(FONT_DRAW_W), (F32)(FONT_DRAW_H), region, fontTex);
                */
                cursorX += font->glyphs[ch].advance;
            }
            if ( *cP == '\n' ) {
                    cursorX = (*it).x;
                    cursorY -= font->maxGlyphHeight*1.1;
                    //if (maxFlag && cursorY + font->maxGlyphHeight > maxY) break;
            }
            cP++;
        }
    }











    // Parse the String


   // glEnd( );*/
}

