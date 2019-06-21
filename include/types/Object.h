#ifndef OBJECT_DEF
#define OBJECT_DEF
#include "General.h"
#include <iostream>
#include "../runtime/ObjectHeader.h"
// This class is mainly used for Heap Object Arrangement


const int MAX_OBJECT_HANDLER_NUM = 100000;
using PObj = void *;

class Object {
    pClass *pkl;
    void *data;
public:
    int size;

    ObjectHeader *header;
    Object *prev, *next;
    Object();
    void Set(pClass *pkl, UInt len, UInt da=0, UInt size=0, char* p=NULL);
    Object(pClass *pkl, UInt len, UInt da=0, UInt size=0); 
    //void Set(pClass*, UInt);
    /*Object(pClass *pkl, UInt len) ; {
        header = new ObjectHeader(pkl, len);
        data = new char[header->length];
    } */

    ~Object();
    void Clear();
    bool changeData(void* ptr);

    void* getIndex(int index, char type='L') ;
    bool putIndex(int index, char* source, int length) ;
    inline int getTotalSize() {
        std::cout << "Size: " << size << " " << header->getLength() << std::endl;
        return size* header->getLength();}

    pClass* getClass() ;
    UInt getTotalLength();
    long* getCookie();
    void* getField(std::string nat) ;
    void* getField(int nat) ;
    inline void* getData() {return data;}
    bool putField(std::string nat, char* source, int length);
    bool putField(int nat, char* source, int length);
};

extern Object ObjectPool[MAX_OBJECT_HANDLER_NUM], *folink, *eolink; 

void InitObjectPool(Object* object);

Object* MakeMultiObjectArray(pClass* pkl, int* arr, int d);
Object* MakeMultiArray(int length, int *arr, int d);
// 请求句柄
Object* requestObject();
// 回收句柄
bool releaseObject(Object* obj);

#endif
