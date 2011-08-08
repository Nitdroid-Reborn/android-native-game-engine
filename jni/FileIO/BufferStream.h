#ifndef BUFFERSTREAM_H
#define BUFFERSTREAM_H

#include "EngineTypes.h"

class BufferStream
{
public:
    BufferStream();
    BufferStream(U8* buffer, U32 bufferLength);

    void setBuffer(U8* buffer, U32 bufferLength);
    bool read(U8* outputBuffer, U32 bytesToRead);
    bool get(U8& byte);
    bool setPosition(U32 position);
    void resetPosition();

private:

    U8* buffer;
    U32 currentPosition;
    U32 bufferLength;
};

#endif // BUFFERSTREAM_H
