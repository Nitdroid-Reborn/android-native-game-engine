#include "BufferStream.h"


BufferStream::BufferStream() {
    this->buffer = NULL;
    this->currentPosition = 0;
    this->bufferLength = 0;
}

BufferStream::BufferStream(void *buffer, U32 bufferLength)
{
    this->buffer = (U8*)buffer;
    currentPosition = 0;
    this->bufferLength=bufferLength;
}

void BufferStream::setBuffer(void *buffer, U32 bufferLength) {
    this->buffer = (U8*)buffer;
    currentPosition = 0;
    this->bufferLength=bufferLength;
}

bool BufferStream::read(void *outputBuffer, U32 bytesToRead) {
    for(int i=0;i<bytesToRead;i++) {
        if(!get(((U8*)outputBuffer)[i])) {
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
