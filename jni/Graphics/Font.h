#ifndef FONT_H
#define FONT_H
#include <Utils/EngineTypes.h>
#include <ContentManager/IContentManager.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>


struct Glyph {
    U8 left;
    U8 top;
    U8 w;
    U8 h;
    F32 u1;
    F32 v1;
    F32 u2;
    F32 v2;
    U8 advance;
};


struct Font {
    TextureHandle fontTexture;
    Glyph glyphs[127];

    U8 maxGlyphWidth;
    U8 maxGlyphHeight;
    void Load(const char* fontPath, U8 fontSize);
    void Dispose();
};

#endif // FONT_H
