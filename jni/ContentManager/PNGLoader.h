#ifndef PNGLOADER_H
#define PNGLOADER_H
#include <libpng/png.h>
#include <GLES/gl.h>
#include "../FileIO/BufferStream.h"

class PNGLoader
{
public:
    PNGLoader();
    GLuint load(const char* filename);

protected:
    static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length);
    BufferStream stream;
};

#endif // PNGLOADER_H
