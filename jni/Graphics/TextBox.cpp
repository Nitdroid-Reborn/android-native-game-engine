#include "TextBox.h"
#include "SpriteBatcher.h"
#include <Graphics/IRenderer.h>
#include <ContentManager/IContentManager.h>

#ifdef ANDROID

#else
#include <Graphics/GLee.h>
#endif

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
    int cursorX;
    int cursorY;
    const char* cP;

    IRenderer*renderer= IRenderer::get();

    cursorX = x;
    cursorY = y;
    cP = (const char*)string;

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


            cursorX += font->glyphs[ch].advance;
        }
        if ( *cP == '\n' ) {
                cursorX = x;
                cursorY -= font->maxGlyphHeight*1.1;
        }
        cP++;
    }
}

