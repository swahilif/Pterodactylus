//
//  GarbageCollect.cpp
//  myJVM
//
//  Created by mac on 2019/5/26.
//  Copyright © 2019 mac. All rights reserved.
//

#include "../include/runtime/GarbageCollect.hpp"
#include "../include/runtime/HeapManager.hpp"
#include "../include/types/ClassFile.h"


// DFS
void dfs(Object* cur){
    Object* tmp;
    stack<Object*> st;
    st.push(cur);
    while(!st.empty()){
        tmp = st.top();
        st.pop();
        tmp->header->marked = 1;
        /*这里应取出tmp对应的pclass里的bool数组并把所有指向handler的指针压栈。此处注意判定marked是否为1*/
        FieldEntry** pF = tmp->getClass()->arrField;
        for (int i = 0; i < tmp->getClass()->field_count; i ++){
#ifdef DEBUG
            cout << "During DFS: field " << i << endl;
#endif
            if (pF[i]->isObject()){
                string nat = pF[i]->GetNameAndDescriptor();
#ifdef DEBUG
                cout << "Attribute " << i << " is object, its name is " << nat << endl;
#endif
                void* ptr = tmp->getField(nat);
                Object* attr = (Object*)*(long*)ptr;
#ifdef DEBUG
                cout << "Get field succeed! Its address is " << (long)attr << endl;
#endif
                if (attr && !attr->header->marked)
                    st.push(attr);
            }
        }
    }
}

// GC: mark
// 步骤：遍历handler pool中所有handler，如果其被指针直接指向的计数器不为0，说明它可为
// root结点，从它开始进行深度优先遍历，把所有关联的都标记上
void GC_mark(Object* folink, Heap* heap){
    //从链表头开始遍历handler
    Object* cur = folink;
    while (cur != NULL){
#ifdef DEBUG
        cout << "Traverse handler " << (long)cur << " ! Its count is:" << cur->header->getCount() << endl;
#endif
        if (cur->header->getCount() > 0)
            dfs(cur);
        cur = cur->next;
    }
}

// GC: sweep
void GC_sweep(Heap* heap, ulong last_obj_length){
    bool end = 0;
    ulong pos = 0;
    ulong total_length = heap->max_heap_size;
    while(1){
        void* data = heap->get_object(pos);
        Object* obj = (Object*)(*(ulong*)data);
        cout << "obj " << (long)obj << endl;
        ulong length = obj->getTotalLength();

#ifdef DEBUG
        cout << "obj " << (long)obj << " Its length is: " << length;
#endif
        bool marked = obj->header->marked;
        if (marked){
#ifdef DEBUG
            cout << " It is kept." << endl;
#endif
            memcpy((void*)(heap->Survivor_offset + heap->Survivor), (const void*)(pos + heap->Eden), length + 8);
            obj->changeData((void*)(heap->Survivor_offset + heap->Survivor + 8));
            heap->Survivor_offset += length + 8;
        }
        else{
#ifdef DEBUG
            cout << " It is swepted." << endl;
#endif
            releaseObject(obj);

        }
        obj->header->marked = 0;
        pos += length + 8;
        if (pos >= total_length - last_obj_length)
            break;
    }
    uchar* tmp;
    tmp = heap->Eden;
    heap->Eden = heap->Survivor;
    heap->Survivor = tmp;
    heap->Eden_offset = heap->Survivor_offset;
    heap->Survivor_offset = 0;
}
