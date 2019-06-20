#include "../include/types/Object.h"
#include "../include/runtime/Field.h"
#include "../include/types/ClassFile.h"
#include <iostream>
#include <queue>
#include <algorithm>
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

Object::Object(pClass *pkl, UInt len, UInt da = 0) {
    header = new ObjectHeader(len, da);
    this->pkl = pkl;
    data = new char[header->getLength() + sizeof(int*)];
    //data = heap.create_obj(header->getLength() + sizeof(int*));
    // Set cookie
    *reinterpret_cast<long*>(data) = reinterpret_cast<long>(this);
    data = reinterpret_cast<Object*>(reinterpret_cast<unsigned long*>(data) + 1);
}

void Object::Set(pClass *pkl, UInt len, UInt da = 0, UInt size = 0, char* pdata=NULL) {
    header = new ObjectHeader(len * size, da);
    this->pkl = pkl;
    if (pdata == NULL) {
        if (da != 0) {
            data = heap.create_obj(len * size + sizeof(int*))
            for (int k = sizeof(int*)); k < len*size + sizeof(int*); k++)            
                data[k] = 0;
        }
        //data = new char[header->getLength() + sizeof(int*)];
        else {
            data = heap.create_obj(header->getLength() + sizeof(int*));
            for (int k = sizeof(int*)); k < header->getLength() + sizeof(int*); k++)            
                data[k] = 0;
        }
    } else data = pdata;

    // Set cookie
    cout << "SET THIS" << this << " " << long(this) << endl;
    *reinterpret_cast<long*>(data) = reinterpret_cast<long>(this);
    data = reinterpret_cast<Object*>(reinterpret_cast<unsigned long*>(data) + 1);
}

Object ObjectPool[MAX_OBJECT_HANDLER_NUM], *folink, *eolink; 
Object::Object() {
    next=prev=NULL;
    data=NULL;
    pkl=NULL;
    header=NULL;
}

void* Object::getIndex(int index) {
    if (index > size ) return false;
    return (char*)data + index * header->getLength();
}

bool putIndex(int index, char* source, int length) {
    char *d = (char*)(data + index * header->getLength());
    if (index > size) return false;
    for (int k=0; k<length; k++)
        d[k] = *(source + k);
    return true;
}

Object::~Object() {
    if (header != NULL)
        delete header;
}

void Object::Clear() {
    delete header;
}

pClass* Object::getClass() {
    return this->pkl;
}

bool Object::changeData(void* ptr) {
    this->data = ptr; 
    return true;
}

void InitObjectPool(Object* pobj) {
    pobj[0].next = pobj + 1;
    pobj[0].prev = NULL;
    for (int i=1; i<MAX_OBJECT_HANDLER_NUM-1; i++) {
        (pobj + i)->prev = pobj + i - 1;
        (pobj + i)->next = pobj + i + 1;
    }
    pobj[MAX_OBJECT_HANDLER_NUM - 1].prev = &pobj[MAX_OBJECT_HANDLER_NUM - 2];
    pobj[MAX_OBJECT_HANDLER_NUM - 1].next = &pobj[MAX_OBJECT_HANDLER_NUM - 2];

    folink = NULL;
    eolink = pobj;
}

Object* requestObject() {
    if (eolink == NULL) return NULL;
    Object* tmp = eolink;
    eolink = eolink->next;
    eolink->prev = NULL;

    tmp->next = folink;
    if (folink != NULL) folink->prev = tmp;
    folink = tmp;
    return tmp;
}

bool releaseObject(Object* obj) {
    if (obj == NULL) return false;
    if (obj->prev != NULL) 
        obj->prev->next = obj->next;
    if (obj->next != NULL)
        obj->next->prev = obj->prev;
    obj->prev = obj->next = NULL;
    obj->next = eolink;
    eolink->prev = obj;
    return true;
}

UInt Object::getTotalLength() {
    if (this->header == NULL) return 0;
    return this->header->getLength();
}

void* Object::getField(int k) {
    char* pdata = (char* )this->data;
    if (this->pkl == NULL) return NULL;
    if (k == -1) {
        return (void*)(pdata - sizeof(int*));
    }
    return (void*)(pdata + this->pkl->byteGrad[k]);
}

long* Object::getCookie() {
    return reinterpret_cast<long*>(*(unsigned long*)this->getField(-1));
}

void* Object::getField(std::string nat) {
    char* pdata = (char* )this->data;
    if (this->pkl == NULL) return NULL;
    return (void*)(pdata + this->pkl->byteGrad[this->pkl->ftp[nat]]);
}

bool Object::putField(std::string nat, char* source, int length) {
    char* pdata = (char* )this->data;
    if (this->pkl == NULL) return false;
    memcpy(pdata + this->pkl->byteGrad[this->pkl->ftp[nat]], source, length);
    return true;
}

bool Object::putField(int k, char* source, int length) {
    char* pdata = (char* )this->data;
    if (this->pkl == NULL) return false;
    memcpy(pdata + this->pkl->byteGrad[k], source, length);
    return true;
}

char* MoveNextPointer(void* p) {
    return (char*)((char*)p + sizeof(int*));
}

queue<pair<void*, pair<int, int>>> handlerQ;
Object* MakeMultiObjectArray(pClass* pkl, int* arr, int d) {
    Object* obj = requestObject();
    assert(obj != NULL);
    int totalSize = 1;
    for (int i=0; i<d; i++) 
        totalSize *= arr[i];
    char* data = heap->create_obj(totalSize + sizeof(int*) * totalSize);
    //*(unsigned long*)data = obj;
    obj->Set(pkl, length, d, arr[0]);
    int *revarr = new int[d];
    revarr[d-1] = 1;
    for (int k=d-2; k>=0; k--)
        revarr[k] = revarr[k+1] * arr[k+1];
    handlerQ.push(make_pair(obj, make_pair(0, 0)));
    while (!handlerQ.empty()) {
        auto& t = handlerQ.pop();
        auto& m = t.second;

        Object *pobj = t.first;

        int dk = m.first;
        int offset = m.second; 

        // 最后一层取消失败
        for (int k=0; k<arr[dk]; k++) {
            Object* ko = requestObject();
            if (dk == d - 1) {
                ko->Set(pkl, length, d-dk-1, arr[dk+1], data + (length + sizeof(int*)) * offset);
                //handlerQ.push(make_pair(ko, make_pair(dk+1, offset + k * revarr[dk])));
            } else {
                ko->Set(pkl, length, d-dk-1, arr[dk+1]);
                handlerQ.push(make_pair(ko, make_pair(dk+1, offset + k * revarr[dk])));
            }
            pobj->putIndex(k, &ko, sizeof(int*));
        }
    }
    delete[] revarr;
    return obj;
}

Object* MakeMultiArray(int length, int *arr, int d) {
    Object* obj = requestObject();
    assert(obj != NULL);
    int totalSize = 1;
    int cookieTotal = 0;
    for (int i=0; i<d; i++) {
        cookieTotal = cookieSize;
        totalSize *= arr[i];
    }
    char* data = heap->create_obj(totalSize + sizeof(int*) * cookieTotal);
    //*(unsigned long*)data = obj;
    obj->Set(NULL, length, d, arr[0]);
    int *revarr = new int[d];
    revarr[d-1] = 1;
    for (int k=d-2; k>=0; k--)
        revarr[k] = revarr[k+1] * arr[k+1];
    handlerQ.push(make_pair(obj, make_pair(0, 0)));
    while (!handlerQ.empty()) {
        auto& t = handlerQ.pop();
        auto& m = t.second;

        Object *pobj = t.first;

        int dk = m.first;
        int offset = m.second; 

        // 最后一层取消失败
        if (dk == d - 1) continue;

        for (int k=0; k<arr[dk]; k++) {
            Object* ko = requestObject();
            if (dk + 1 != d - 1) {
                ko->Set(NULL, length, d-dk-1, arr[dk+1]);
                handlerQ.push(make_pair(ko, make_pair(dk+1, offset + k * revarr[dk])));
            } else {
                ko->Set(NULL, length, d-dk-1, arr[dk+1], data + offset * length + (offset * sizeof(int*) / arr[dk-1]));
                handlerQ.push(make_pair(ko, make_pair(dk+1, offset)));
            }
            pobj->putIndex(k, &ko, sizeof(int*));
        }
    }
    delete[] revarr;
    return obj;
}
