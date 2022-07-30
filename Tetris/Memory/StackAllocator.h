#pragma once
#include "definitions.h"

class StackAllocator{
public:
    typedef u64 Marker;

    explicit StackAllocator(u32 stack_bytes);

    void* alloc(u32 size_bites);

    Marker getMarker();

    void freeToMarker(Marker marker);

    void clear();
private:
    Marker stack_top;
    Marker stack_limit;
    Marker stack_origin;
};