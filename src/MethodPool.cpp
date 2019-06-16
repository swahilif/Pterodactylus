#include "../include/runtime/MethodPool.h"
#include <iostream>
using std::cout;
using std::endl;

PureCode::PureCode() {current_pos=0; code_length = 0; pData = NULL;}
PureCode::PureCode(CODE* pCode) {
    pData = new HexCode[pCode->code_length + 1];
    current_pos = 0;

    max_stack = pCode->max_stack;
    max_locals = pCode->max_locals;
    code_length = pCode->code_length;
    attributes_count = pCode->attributes_count;

    cout << "Enter into the pCode!" << endl;
    cout << max_stack << " " << max_locals << " " << code_length << endl;

    for (auto x = 0; x < code_length; x++) {
        *(pData+x) = *(pCode->code+x);
        cout << "x:"<<x<<", HEX:"<<int(*(pData+x))<<endl;
    }
    cout << "END!!!p" << endl;
}

UShort PureCode::GetMaxStack() {return max_stack;}
UShort PureCode::GetMaxLocals() {return max_locals;}
UInt PureCode::GetCodeLength() {return code_length;}

PureCode::~PureCode() {delete[] pData;}

HexCode PureCode::GetNextCode() {
    try {
        if (current_pos + 1 >= code_length)
            throw "Access denied! The hex code read exceeds the maximum length.";
        return *(pData+(current_pos++));
    } catch (char *error_info) {
#ifdef DEBUG_CLASS
        cout << error_info << endl;
#endif
    }
    return 0;
}

void PureCode::SetCurrentPos(UShort index) {
    try {
        if (index >= code_length)
            throw "Access denied! The hex code read exceeds the maximum length.";
        else if (index < 0) 
            throw "Access denied! The index can't be negative!";
        current_pos = index;
    } catch (char *error_info) {
#ifdef DEBUG_CLASS
        cout << error_info << endl;
#endif
    }
}

HexCode PureCode::GetCurrentPos() const {
    return current_pos;
}

HexCode PureCode::GetIndexCode(UShort index) const{
    try {
        if (index >= code_length)
            throw "Access denied! The hex code read exceeds the maximum length.";
        else if (index < 0) 
            throw "Access denied! The index can't be negative!";
        return *(pData+index);
    } catch (char *error_info) {
#ifdef DEBUG_CLASS
        cout << error_info << endl;
#endif
        return 0;
    }
}

// [ClassName] PureCodePool
// 管理PureCode用的索引结构
PureCodePool::PureCodePool(UInt poolLength) {
    CodePool.resize(poolLength);
    PoolLength = poolLength;
    for (auto k = poolLength - 1; k >= 1; k--) {
        VacantIndexStack.push(k);
    }
}

UInt PureCodePool::Reserve(CODE* pCode) { // UInt _codeLength, UBytePtr _codeString) {
    if (VacantIndexStack.empty()) // 分配PureCode失败
        return 0;
    UInt reserved_slot = VacantIndexStack.top();
    VacantIndexStack.pop();
    cout << reserved_slot << "!" << endl;
    CodePool[reserved_slot] = new PureCode(pCode); //_codeLength, _codeString);
    cout << "HAHA" << endl;
    return reserved_slot;
}

DataType PureCodePool::Access(UInt access_slot) {
    try {
        if (access_slot >= PoolLength)
            throw "[PureCodePool] Invalid Access!";
        return CodePool[access_slot];
    } catch (char *error_info) {
#ifdef DEBUG_CLASS
        cout << error_info << endl;
#endif
        return NULL;
    }        
}

// [MethodName] PureCodePool::Release
// 返回是否释放成功
StatusCode PureCodePool::Release(UInt released_slot) {
    if (CodePool[released_slot] == NULL)
        return 0;

    delete CodePool[released_slot];
    VacantIndexStack.push(released_slot);
    return 1;
}

PureCodePool::~PureCodePool() {
    for (auto k = 0; k < PoolLength; k++)
        Release(k);
}

bool MethodEntry::GetAccessFlags(string flag) {
    if (flag == "ACC_STATIC")
        return ACC_STATIC;
    if (flag == "ACC_FINAL")
        return ACC_FINAL;
    if (flag == "ACC_PUBLIC")
        return ACC_PUBLIC;
    if (flag == "ACC_PRIVATE")
        return ACC_PRIVATE;
    if (flag == "ACC_ABSTRACT")
        return ACC_ABSTRACT;
    if (flag == "ACC_NATIVE")
        return ACC_NATIVE;
    return false;
}
PureCodePool GenCodePool(100);
MethodEntry::MethodEntry() {}

MethodEntry::MethodEntry(METHODINFO* pmi, class_attribute* pkl) {
    ACC_PUBLIC = pmi->ACC_PUBLIC;
    ACC_PRIVATE = pmi->ACC_PRIVATE;
    ACC_PROTECTED = pmi->ACC_PROTECTED;
    ACC_STATIC = pmi->ACC_STATIC;
    ACC_FINAL = pmi->ACC_FINAL;
    //ACC_SYNCHRONIZE = pmi->ACC_SYNCHRONIZE
    ACC_BRIDGE = pmi->ACC_BRIDGE;
    ACC_VARARGS = pmi->ACC_VARARGS;
    ACC_NATIVE = pmi->ACC_NATIVE;
    ACC_ABSTRACT = pmi->ACC_ABSTRACT;
    ACC_STRICTFP = pmi->ACC_STRICTFP;
    ACC_SYNTHETIC = pmi->ACC_SYNTHETIC; 

    printf("ACCESS PRIVELIGFE :%d %d %d\n", ACC_PUBLIC, ACC_PRIVATE, ACC_PROTECTED);

    auto info = pmi->get_info(pkl);

    name = info.first;
    descriptor = info.second;

    name_and_descriptor = name + ":" + descriptor; 
    cout << name_and_descriptor << "NAT" << endl;

    attribute_count = pmi->attributes_count;
    cout << attribute_count << "attr_count" << endl;
    cout << (CODE*)pmi->attributes[pmi->get_code_index()] << "[[[attr_count" << endl;

    UInt  t = GenCodePool.Reserve((CODE*)pmi->attributes[pmi->get_code_index()]);
    cout << t << endl;
    method_res_pos = t;
    cout << "G" << endl;
}

NAT MethodEntry::GetName() {return name;}
NAT MethodEntry::GetDescriptor() {return descriptor;}
NAT MethodEntry::GetNameAndDescriptor() {return name_and_descriptor;}

UInt MethodEntry::GetMethodResPos() { return method_res_pos; }
