#ifndef OBJECT_HEADER_H
#define OBJECT_HEADER_H
#include "../types/General.h"
class Object;

// ObjectHeader 对象头的相关信息
// cnt 被root 

class ObjectHeader {
    UInt length;   
    int cnt;
    bool isArr;
    friend class Object;
public:
    bool marked;
    ObjectHeader();
    ObjectHeader(UInt _len); //:pkl(_pkl), length(_len) {}
    UInt getLength() ; //{return length;}
    int getCount();
    int changeCnt(int del);
};
#endif
