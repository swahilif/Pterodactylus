#include "../include/types/Object.h"
#include "../include/runtime/Field.h"
#include "../include/types/ClassFile.h"

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

void* Object::getField(std::string nat) {
    char* pdata = (char* )this->data;
    return (void*)(pdata + this->getClass()->byteGrad[this->getClass()->ftp[nat]]);
}
