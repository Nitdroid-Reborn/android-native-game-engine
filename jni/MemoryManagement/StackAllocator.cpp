#include "StackAllocator.h"

StackAllocator::StackAllocator(U32 stackSizeInBytes)
{
    stackBase = new U8[stackSizeInBytes];
    stackTop = stackBase + stackSizeInBytes;
    stackPointer = stackBase;
}

StackAllocator::~StackAllocator() {
    delete[] stackBase;
}

void* StackAllocator::Alloc(U32 sizeBytes) {
    if(stackPointer+sizeBytes<=stackTop) {
        U8* mem = stackPointer;
        stackPointer+=sizeBytes;

        return (void*)mem;
    }
    return 0;
}

void* StackAllocator::AllocAligned(U32 sizeBytes, U32 aligment) {
    U32 expandedSize = sizeBytes + aligment;
    if(stackPointer+expandedSize<=stackTop) {
        U32 rawAddress = (U32)stackPointer;
        stackPointer+=expandedSize;

        U32 mask = (aligment - 1);
        U32 misalignment = (rawAddress & mask);
        U32 adjustment = aligment - misalignment;

        return (void*)(rawAddress+adjustment);
    }

    return 0;
}

StackAllocator::MemoryMarker StackAllocator::GetMarker() {
    return (U32)stackPointer;
}

void StackAllocator::FreeToMarker(StackAllocator::MemoryMarker marker) {
    stackPointer = (U8*)marker;
}

void StackAllocator::Clear() {
    stackPointer = stackBase;
}
