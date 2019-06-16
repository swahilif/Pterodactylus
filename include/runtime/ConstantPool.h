#ifndef CONSTANT_POOL_DEF
#define CONSTANT_POOL_DEF

#include <stdio.h>
#include "../types/General.h"
#include "ClassBase.h"
#include <string.h>
#include <ctype.h>

/*
 * Please move from the exit rows if you are unwilling and unable to perform the necessary actions without injury

*/
class ConstantPool;

class ConstantPoolMetaType {
protected:
    int instance_type;
public:
    //virtual int GetInstanceType() ;//{return instance_type;}
    //
    ConstantPoolMetaType() {instance_type=0;}
    int GetInstanceType() { return instance_type;}
    virtual void* GetValue(){return NULL;}
    virtual NAT GetNameAndType(){return NAT();}// {return "";}
    virtual ~ConstantPoolMetaType() {}
    virtual int resolved(ConstantPool*) {return 0;}
};

class ConstantPoolRefType: public ConstantPoolMetaType {
    NAT nat;
    UShort class_index, entry_index;
public:
    ConstantPoolRefType(UShort _class_index, UShort _entry_index, int _instance_type) ;
    int resolved(ConstantPool*);
    void *GetValue();
    NAT GetNameAndType();
    ~ConstantPoolRefType();
    friend class ConstantPool;
};

class ConstantPoolValueType: public ConstantPoolMetaType {
    void* value;
public:
    ConstantPoolValueType(void* _value, int _instant_type) ;
    ~ConstantPoolValueType() ;
    int resolved(ConstantPool*);
    void* GetValue() ;
    friend class ConstantPool;
};

class ConstantPoolNAT: public ConstantPoolMetaType {
    UInt name_index, descriptor_index;
    string nat;
public:
    ConstantPoolNAT(UInt _name_index, UInt _descriptor_index) ;
    ~ConstantPoolNAT();
    void* GetValue();
    int resolved(ConstantPool* pcp);
    NAT GetNameAndType();
    friend class ConstantPool;
};

class ConstantPool {
    ConstantPoolMetaType** cpm;
    UShort constant_pool_count;
public:
    ConstantPool(CONSTANT_METATYPE** pcm, UShort ConstantPoolCount) ; 
    ~ConstantPool();
    ConstantPoolMetaType* GetConstantPoolItem(int index); 
    bool ResolveNAT();
    bool ResolveRef();
    bool ResolveCLS();
};
#endif
