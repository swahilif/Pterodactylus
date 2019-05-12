#include "General.h"
class Object;
class ObjectHeader {
    pClass* pkl;
    UInt length;   
    friend class Object;
public:
    ObjectHeader(pClass* _pkl, UInt _len):pkl(_pkl), length(len) {}
};
