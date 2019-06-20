#include "../include/runtime/ObjectHeader.h"
#include <iostream>
ObjectHeader::ObjectHeader(UInt _len):length(_len) {marked = false;cnt = 0;}
UInt ObjectHeader::getLength() {return length;}
int ObjectHeader::getCount() {return this->cnt;}
ObjectHeader::ObjectHeader() {this->cnt=0; this->length=0; }
int ObjectHeader::changeCnt(int delta) {this->cnt += delta; return 1;}
