#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H
#include "../EngineTypes.h"

class StackAllocator
{
public:
    typedef U32 MemoryMarker;

    explicit StackAllocator(U32 stackSizeInBytes);

    ~StackAllocator();

    void* Alloc(U32 sizeBytes);

    void* AllocAligned(U32 sizeBytes, U32 aligment);

    MemoryMarker GetMarker();

    void FreeToMarker(MemoryMarker marker);

    void Clear();

private:
    U8* stackPointer;
    U8* stackBase;
    U8* stackTop;
};

#endif // STACKALLOCATOR_H
