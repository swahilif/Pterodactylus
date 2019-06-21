//
//  HeapManager.cpp
//  myJVM
//
//  Created by mac on 2019/5/26.
//  Copyright © 2019 mac. All rights reserved.
//

#include "../include/runtime/HeapManager.hpp"
#include <iostream>
using namespace std;

Heap* heap;

Heap::Heap(ulong length){
    Eden = new uchar[length];
    Survivor = new uchar[length];
    //cout << "Eden Address: " << (long)Eden << endl;
    memset(Eden, 0, length);
    memset(Survivor, 0, length);
    max_heap_size = length;
    Eden_offset = 0;
    Survivor_offset = 0;
    start = (long)Eden;
}

void* Heap::create_obj(ulong length){
    uchar* point = Eden + Eden_offset;
    Eden_offset += length;
    cout << length << " bits required! Current ptr pos: " << Eden_offset << endl;
    cout << "Required Heap Length: " << length << endl;
    cout << "Current Pos: " << Eden_offset << endl;
    /*
    if (Eden_offset > max_heap_size){
        cout << "Garbage Collection triggered!" << endl;
        GC_mark(folink, heap);
        GC_sweep(heap);
    }
     */
    return (void*) point;
}

bool Heap::check_space(ulong length){
    cout << "Eden offset: " << Eden_offset << endl;
    if (Eden_offset + length + 8 >= max_heap_size){
        cout << "Garbage Collection triggered!" << endl;
        GC_mark(folink, heap);
        GC_sweep(heap, length + 8);
        if (Eden_offset + length + 8 >= max_heap_size)
            return false;
        return true;
    }
    else
        return true;
}

void* Heap::get_object(ulong pos){
    return (void*)(Eden + pos);
}

