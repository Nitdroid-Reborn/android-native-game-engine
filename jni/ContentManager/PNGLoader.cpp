#include "PNGLoader.h"
#include "FileIO/IFileIO.h"
#include <Utils/Utils.h>

PNGLoader::PNGLoader()
{
}


void PNGLoader::pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length) {
    if(png_ptr->io_ptr == NULL)
          return;

    BufferStream* stream = (BufferStream*)png_ptr->io_ptr;
    if(!stream->read(data, length))
        return;
}

bool PNGLoader::Load(const char *filename, U8* &imageData, U16 &width,
                     U16 &height, U8& colorDepth, bool &alpha) {

    U32 len = IFileIO::get()->GetSize(filename);
    U8* buffer = new U8[len];

    IFileIO::get()->Read(filename, buffer, len);

    stream.setBuffer(buffer, len);

    //header for testing if it is a png
    png_byte header[8];

    //read the header
    stream.read(header, 8);

    //test if png
    int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        delete[] buffer;
        buffer = NULL;
        LOGE("Not a png file : %s", filename);
        return false;
      }
    LOGI("This is png file: %s", filename);

     //create png struct
      png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
          NULL, NULL);
      if (!png_ptr) {
        delete[] buffer;
        buffer = NULL;
        LOGE("Unable to create png struct : %s", filename);
        return false;
      }

      //create png info struct
      png_infop info_ptr = png_create_info_struct(png_ptr);
      if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        LOGE("Unable to create png info : %s", filename);
        delete[] buffer;
        buffer = NULL;
        return false;
      }

      //create png info struct
      png_infop end_info = png_create_info_struct(png_ptr);
      if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        LOGE("Unable to create png end info : %s", filename);
        delete[] buffer;
        buffer = NULL;
        return false;
      }

      //png error stuff, not sure libpng man suggests this.
      if (setjmp(png_jmpbuf(png_ptr))) {
        delete[] buffer;
        buffer = NULL;
        LOGE("Error during setjmp : %s", filename);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return false;
      }

      //init png reading
      //png_init_io(png_ptr, fp);
      png_set_read_fn(png_ptr, &stream, pngReadCallback);

      //let libpng know you already read the first 8 bytes
      png_set_sig_bytes(png_ptr, 8);

      // read all the info up to the image data
      png_read_info(png_ptr, info_ptr);

      //variables to pass to get info
      int bit_depth, color_type;
      png_uint_32 twidth, theight;

      // get info about png
      png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
          NULL, NULL, NULL);

      width = twidth;
      height = theight;

      if(color_type == PNG_COLOR_TYPE_RGB) {
          colorDepth = 8;
          alpha = false;
      }
      else {
          colorDepth = 8;
          alpha = true;
      }

      //update width and height based on png info

      // Update the png info struct.
      png_read_update_info(png_ptr, info_ptr);

      // Row size in bytes.
      int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

      // Allocate the image_data as a big block, to be given to opengl
      imageData = new png_byte[rowbytes * height];
      if (!imageData) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        LOGE("Unable to allocate image_data while loading %s ", filename);
        delete[] buffer;
        buffer = NULL;
        return false;
      }

      //row_pointers is for pointing to image_data for reading the png with libpng
      png_bytep *row_pointers = new png_bytep[height];
      if (!row_pointers) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete[] imageData;
        LOGE("Unable to allocate row_pointer while loading %s ", filename);
        delete[] buffer;
        buffer = NULL;
        return false;
      }
      // set the individual row_pointers to point at the correct offsets of image_data
    for (int i = 0; i < height; ++i)
        row_pointers[height - 1 - i] = imageData + i * rowbytes;

      //read the png into image_data through row_pointers
      png_read_image(png_ptr, row_pointers);


      //clean up memory and close stuff
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

      delete[] row_pointers;
      delete[] buffer;

      return true;
}
