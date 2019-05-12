#include "../include/runtime/ObjectHeader.h"
ObjectHeader::ObjectHeader(pClass* _pkl, UInt _len):pkl(_pkl), length(_len) {}
pClass* ObjectHeader::getClass() {return pkl;}
UInt ObjectHeader::getLength() {return length;}
