#include "Font.h"
#include <Utils/Log.h>

U16 nextPowerOf2(U16 a) {
    U16 v=1;
    while(v<a)v<<=1;

    return v;
}

void Font::Load(const char *fontPath, U8 fontSize) {
    FT_Library library;
    FT_Init_FreeType( &library );

    FT_Face face;

    if(FT_New_Face( library, fontPath, 0, &face ))
        Logger::Log("pupa");

    FT_Set_Char_Size( face, fontSize << 6, fontSize << 6, 96, 96);

    maxGlyphHeight=maxGlyphWidth=0;
    for(char c=32;c<126;c++) {
        FT_Load_Glyph( face, FT_Get_Char_Index( face, c ), FT_LOAD_DEFAULT );


        FT_Glyph glyph;
        FT_Get_Glyph( face->glyph, &glyph );

        // Convert The Glyph To A Bitmap.
        FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

        // This Reference Will Make Accessing The Bitmap Easier.
        FT_Bitmap& bitmap=bitmap_glyph->bitmap;

        if(bitmap.width>maxGlyphWidth)maxGlyphWidth=bitmap.width;
        if(bitmap.rows>maxGlyphHeight)maxGlyphHeight=bitmap.rows;
    }

    U16 textureWidth = nextPowerOf2(maxGlyphWidth*10);
    U16 textureHeight = nextPowerOf2(maxGlyphHeight*10);

    unsigned char* data = new unsigned char[2*textureWidth*textureHeight];

    memset(data, 0, 2*textureWidth*textureHeight);

    for(char c=0;c<95;c++) {
        int xOffset = c%10;
        int yOffset = c/10;

        char ch = c+32;

        FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT );


        FT_Glyph glyph;
        FT_Get_Glyph( face->glyph, &glyph );

        // Convert The Glyph To A Bitmap.
        FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

        // This Reference Will Make Accessing The Bitmap Easier.
        FT_Bitmap& bitmap=bitmap_glyph->bitmap;

        glyphs[ch].h = bitmap.rows;
        glyphs[ch].w = bitmap.width;
        glyphs[ch].left = bitmap_glyph->left;
        glyphs[ch].top = bitmap_glyph->top;
        glyphs[ch].advance = face->glyph->advance.x >> 6;

        float xOff = (((float)(c%10))*maxGlyphWidth)/textureWidth;
        float yOff = (textureHeight-(((float)(c/10))*maxGlyphHeight))/textureHeight;
        glyphs[ch].u1 = xOff;
        glyphs[ch].v1 = yOff - (float)glyphs[ch].h/textureHeight;
        glyphs[ch].u2 = xOff + (float)glyphs[ch].w/textureWidth;
        glyphs[ch].v2 = yOff;



        for(int y=0;y<bitmap.rows;y++) {
            for(int x=0;x<bitmap.width;x++) {

                int index = (xOffset*maxGlyphWidth + x) + textureWidth*((textureHeight-1) - (yOffset*maxGlyphHeight + y));


                data[2*index] = bitmap.buffer[x+bitmap.width*y];
                data[2*index+1] = bitmap.buffer[x+bitmap.width*y];
            }
        }
    }


    TextureInfo info;
    info.format = TextureInfo::LUMINANCE_ALPHA;
    info.width = textureWidth;
    info.height = textureHeight;

    fontTexture = IContentManager::get()->GetTextureManager()->GetTexture(data, info);


    FT_Done_Face(face);
    FT_Done_FreeType(library);

    delete[] data;

    Logger::Log("Loaded %d %d", maxGlyphWidth, maxGlyphHeight);
}

void Font::Dispose() {
    if(!fontTexture.IsNull())
        IContentManager::get()->GetTextureManager()->ReleaseTexture(fontTexture);
}
