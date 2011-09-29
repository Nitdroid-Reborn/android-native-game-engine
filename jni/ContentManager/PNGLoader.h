#ifndef PNGLOADER_H
#define PNGLOADER_H
#include <png.h>
#include <GLES/gl.h>
#include "../FileIO/BufferStream.h"

class PNGLoader
{
public:
    PNGLoader();
    bool Load(const char* filename, U8*& imageData, U16& width, U16& height, U8& colorDepth, bool& alpha);

protected:
    static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length);
    BufferStream stream;
};

#endif // PNGLOADER_H
