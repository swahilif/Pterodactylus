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
    void Object::Set(pClass *pkl, UInt len, UInt da=0, UInt size=0);
    Object(pClass *pkl, UInt len, UInt da=0, UInt size=0); 

    ~Object();
    void Clear();
    void Jump(int offset);
    bool changeData(void* ptr);

    void* getIndex(int index) ;
    bool putIndex(int index, char* source, int length) ;

    pClass* getClass() ;
    UInt getTotalLength();
    long* getCookie();
    void* getField(std::string nat) ;
    void* getField(int nat) ;
    bool putField(std::string nat, char* source, int length);
    bool putField(int nat, char* source, int length);
};

extern Object ObjectPool[MAX_OBJECT_HANDLER_NUM], *folink, *eolink; 

void InitObjectPool(Object* object);

// 请求句柄
Object* requestObject();
// 回收句柄
bool releaseObject(Object* obj);

Object* MakeMultiArray(pClass* pkl, int* arr, int d);
Object* MakeMultiObjectArry(int length, int *arr, int d);

bool InitializeObject()

#endif
