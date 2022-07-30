#include "StackAllocator.h"
#include <stdlib.h>

StackAllocator::StackAllocator(u32 stack_bytes){
    stack_top = (Marker)malloc(stack_bytes);
    stack_origin = stack_top;
    stack_limit = stack_top + stack_bytes;
    ASSERT(stack_limit > stack_top);
}

void* StackAllocator::alloc(u32 size_bytes){
    void* mem_addr;
    ASSERT(stack_top + size_bytes <= stack_limit);
    if(stack_top + size_bytes <= stack_limit){
        mem_addr = (void*)stack_top;
        stack_top = stack_top + size_bytes;
    } else {
        //TODO(Bradley): We need to come up with some method to free this
        mem_addr = malloc(size_bytes);
        //OutputDebugStringA("Stack mem overflow\n");
    }
    return mem_addr;
}

StackAllocator::Marker StackAllocator::getMarker(){
    return stack_top;
}

void StackAllocator::freeToMarker(StackAllocator::Marker marker){
    stack_top = marker;
}

void StackAllocator::clear(){
    stack_top = stack_origin;
}



