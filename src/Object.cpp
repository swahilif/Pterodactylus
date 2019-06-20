#include "../include/types/Object.h"
#include "../include/runtime/Field.h"
#include "../include/types/ClassFile.h"
#include "../include/runtime/HeapManager.hpp"
#include <iostream>
using std::cout;
using std::endl;

Object::Object(pClass *pkl, UInt len) {
    header = new ObjectHeader(len);
    this->pkl = pkl;
    //data = new char[header->getLength() + sizeof(int*)];
    data = heap->create_obj(header->getLength() + sizeof(int*));
    // Set cookie
    *reinterpret_cast<long*>(data) = reinterpret_cast<long>(this);
    data = reinterpret_cast<Object*>(reinterpret_cast<unsigned long*>(data) + 1);
}

void Object::Set(pClass *pkl, UInt len) {
    header = new ObjectHeader(len);
    this->pkl = pkl;
    // 此处是为了gc触发的时候不回收此handler
    this->header->changeCnt(1);
    //data = new char[header->getLength() + sizeof(int*)];
    data = heap->create_obj(header->getLength() + sizeof(int*));
    // Set cookie
    *reinterpret_cast<long*>(data) = reinterpret_cast<long>(this);
    data = reinterpret_cast<Object*>(reinterpret_cast<unsigned long*>(data) + 1);
    this->header->changeCnt(-1);
}

Object ObjectPool[MAX_OBJECT_HANDLER_NUM], *folink, *eolink; 
Object::Object() {
    next=prev=NULL;
    data=NULL;
    pkl=NULL;
    header=NULL;
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
    if (obj->prev == NULL){
        folink = obj->next;
    }
    obj->prev = obj->next = NULL;
    obj->next = eolink;
    eolink->prev = obj;
    eolink = obj;
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
    int byte_pos = this->pkl->byteGrad[this->pkl->ftp[nat]];
    //cout << "Byte Pos of this attr: " << byte_pos << endl;
    return (void*)(pdata + this->pkl->byteGrad[this->pkl->ftp[nat]]);
}

bool Object::putField(std::string nat, char* source, int length) {
    char* pdata = (char*)this->data;
    if (this->pkl == NULL) return false;
    int test = this->pkl->byteGrad[this->pkl->ftp[nat]];
    memcpy(pdata + test, source, length);
    return true;
}

bool Object::putField(int k, char* source, int length) {
    char* pdata = (char* )this->data;
    if (this->pkl == NULL) return false;
    memcpy(pdata + this->pkl->byteGrad[k], source, length);
    return true;
}
