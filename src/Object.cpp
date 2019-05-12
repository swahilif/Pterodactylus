#include "../include/types/Object.h"
Object::Object(pClass *pkl, UInt len) {
    header = new ObjectHeader(pkl, len);
    data = new char[header->length];
}

Object::~Object() {
    delete data;
    delete header;
}

pClass* Object::getClass() {
    return this->header->getClass();
}

UInt Object::getTotalLength() {
    return this->header->getLength();
}

