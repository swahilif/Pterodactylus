//
//  HeapManager.hpp
//  myJVM
//
//  Created by mac on 2019/5/26.
//  Copyright © 2019 mac. All rights reserved.
//

#include <stdio.h>
#include "../types/Object.h"
#include "ObjectHeader.h"
#include "GarbageCollect.hpp"
#include <string.h>

#ifndef HEAPDEF
#define HEAPDEF
using ulong = unsigned long;
using uchar = unsigned char;
class Heap{
public:
    ulong max_heap_size;
    ulong Survivor_offset;
    ulong Eden_offset;
    Heap(ulong length);
    void* create_obj(ulong length);
    void* get_object(ulong pos);
    uchar* Eden;
    uchar* Survivor;
    bool check_space(ulong len);
    ulong start;
    //void gc_copy(ulong eden_pos, ulong survivor_pos, int length);
};
extern Heap* heap;
#endif

