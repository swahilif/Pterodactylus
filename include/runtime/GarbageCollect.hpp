//
//  GarbageCollect.hpp
//  myJVM
//
//  Created by mac on 2019/5/26.
//  Copyright © 2019 mac. All rights reserved.
//
#include "../types/Object.h"
#include "../runtime/javap.h"
#include <string.h>
#include <string>
#include <stack>
#include <vector>
using namespace std;

class Heap;
class FieldEntry;
void GC_mark(Object* folink, Heap* heap);
void GC_sweep(Heap* heap, ulong last_obj_length);
