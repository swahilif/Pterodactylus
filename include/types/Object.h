#include "General.h"
#include "../runtime/ObjectHeader.h"
// This class is mainly used for Heap Object Arrangement

using PObj = void *;

class Object {
    ObjectHeader *header;
    void* data;
public:
    Object(pClass *pkl, UInt len) ; /*{
        header = new ObjectHeader(pkl, len);
        data = new char[header->length];
    } */

    ~Object() ;/*
        delete data;
        delete header;
    } */

    pClass* getClass() ;
    UInt getTotalLength() ;
    void* getField(std::string nat) ;
};


