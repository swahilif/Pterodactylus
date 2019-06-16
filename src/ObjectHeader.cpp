#include "../include/runtime/ObjectHeader.h"
ObjectHeader::ObjectHeader(UInt _len):length(_len) {}
UInt ObjectHeader::getLength() {return length;}
int ObjectHeader::getCount() {return this->cnt;}
ObjectHeader::ObjectHeader() {this->cnt=0; this->length=0; }
int ObjectHeader::changeCnt(int delta) {this->cnt += delta; return 1;}
