#include "BufferStream.h"


BufferStream::BufferStream() {
    this->buffer = NULL;
    this->currentPosition = 0;
    this->bufferLength = 0;
}

BufferStream::BufferStream(U8 *buffer, U32 bufferLength)
{
    this->buffer = buffer;
    currentPosition = 0;
    this->bufferLength=bufferLength;
}

void BufferStream::setBuffer(U8 *buffer, U32 bufferLength) {
    this->buffer = buffer;
    currentPosition = 0;
    this->bufferLength=bufferLength;
}

bool BufferStream::read(U8 *outputBuffer, U32 bytesToRead) {
    for(int i=0;i<bytesToRead;i++) {
        if(!get(outputBuffer[i])) {
            return false;
        }
    }
    return true;
}

bool BufferStream::setPosition(U32 position) {
    if(position < bufferLength) {
        currentPosition = position;
        return true;
    }
    return false;
}

void BufferStream::resetPosition() {
    currentPosition = 0;
}

bool BufferStream::get(U8 &byte) {
    if(currentPosition<bufferLength) {
        byte = buffer[currentPosition];
        currentPosition++;
        return true;
    }

    return false;
}
