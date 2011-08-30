#ifndef BUFFERSTREAM_H
#define BUFFERSTREAM_H

#include <Utils/EngineTypes.h>

class BufferStream
{
public:
    BufferStream();
    BufferStream(void* buffer, U32 bufferLength);

    void setBuffer(void* buffer, U32 bufferLength);
    bool read(void* outputBuffer, U32 bytesToRead);
    bool get(U8& byte);
    bool setPosition(U32 position);
    void resetPosition();

private:
    U8* buffer;
    U32 currentPosition;
    U32 bufferLength;
};

#endif // BUFFERSTREAM_H
