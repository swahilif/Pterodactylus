#ifndef OBJECT_DEF
#define OBJECT_DEF
#include "General.h"
#include "../runtime/ObjectHeader.h"
// This class is mainly used for Heap Object Arrangement


const int MAX_OBJECT_HANDLER_NUM = 100000;
using PObj = void *;

class Object {
    pClass *pkl;
    void *data;
public:
    ObjectHeader *header;
    Object *prev, *next;
    Object();
    Object(pClass *pkl, UInt len) ; /*{
        header = new ObjectHeader(pkl, len);
        data = new char[header->length];
    } */

    ~Object();
    void Clear();
    bool changeData(void* ptr);

    pClass* getClass() ;
    UInt getTotalLength();
    void* getField(std::string nat) ;
    bool putField(std::string nat, char* source, int length);
};

extern Object ObjectPool[MAX_OBJECT_HANDLER_NUM], *folink, *eolink; 

void InitObjectPool(Object* object);

// 请求句柄
Object* requestObject();
// 回收句柄
bool releaseObject(Object* obj);

#endif
