#include "WaveSound.h"
#include <FileIO/IFileIO.h>
#include <FileIO/BufferStream.h>
#include <Utils/Utils.h>
#include <string.h>
WaveSound::WaveSound()
{

}
void readText(BufferStream &stream, int size, char* buffer)
{
    for(int i = 0; i < size; i++)
    {
        stream.read(&buffer[i], 1);
    }
    buffer[size]='\0';
}

bool WaveSound::Load(const char *filename, U16*& data, U32& sampleCount) {
    U32 len = IFileIO::get()->GetFileSize(filename);
    U8* buffer = new U8[len];

    IFileIO::get()->ReadFile(filename, buffer, len);

    BufferStream stream(buffer, len);

    char textBuffer[16];


    readText(stream, 4, textBuffer);

    if(strcmp(textBuffer, "RIFF") != 0) {
        Logger::Log(1, "Not a riff file");
        return false;
    }

    stream.read(&fileSize, sizeof(U32));

    readText(stream, 4, textBuffer);
    if(strcmp(textBuffer, "WAVE") != 0) {
        Logger::Log(1, "Not a wave file");
        return false;
    }

    readText(stream, 4, textBuffer);
    if(strcmp(textBuffer, "fmt ") != 0) {
        Logger::Log(1, "No format chunk");
        return false;
    }

    U32 tmp;
    stream.read(&tmp, sizeof(U32));

    if(tmp != 16) {
        Logger::Log(1, "Invalid format chunk");
        return false;
    }

    stream.read(&compressionCode, sizeof(U16));

    if(compressionCode != 1) {
        Logger::Log(1, "Invalid compression");
        return false;
    }

    stream.read(&channels, sizeof(U16));
    stream.read(&sampleRate, sizeof(U32));
    stream.read(&avgBytesPerSecond, sizeof(U32));
    stream.read(&blockAlign, sizeof(U16));
    stream.read(&significantBitsPerSample, sizeof(U16));

    ASSERT(channels == 1, "Only 1 channel data");
    ASSERT(sampleRate == 44100, "Only 44100Hz data");
    ASSERT(significantBitsPerSample == 16, "Only 16bits data");

    U8 temp;
    do {
        readText(stream, 4, textBuffer);
        if(strcmp(textBuffer, "data") != 0) {
            stream.read(&dataSize, sizeof(U32));
            for(int i=0;i<dataSize;i++) {
                stream.read(&temp, 1);
            }
        }
        else {
            break;
        }
    }while(1);


    stream.read(&dataSize, sizeof(U32));

    sampleCount = dataSize / blockAlign;

   // data = new U16*[channels];

  //  for(U16 i = 0; i < channels; i++)
    data = new U16[sampleCount];

    for(U32 sample = 0; sample < sampleCount; sample++) {
           stream.read(&data[sample], sizeof(U16));
    }
    return true;
}



WaveSound::~WaveSound() {

}

