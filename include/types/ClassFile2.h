#ifndef CLASSFILE_DEF
#define CLASSFILE_DEF
#include "../runtime/VirtualTable.h"
#include "../runtime/ClassBase.h"
#include "../runtime/ConstantPool.h"
#include "../runtime/Field.h"
#include "../runtime/StaticMethodTable.h"
#include "General.h"
#include <assert.h>

// [ClassName] pClass
// 解析后的类存放地址

class pClass {
    UBoolean is_loaded_;
public:
    VirtualTable* vtp;
    StaticMethodTable* smtp; // 针对类中所有的静态函数的Table

    pClass* pFatherClass;
    MethodEntry** arrMethod; // 类中所有函数的对应MethodEntry*
    FieldEntry** arrField;
    FieldEntry** staticField;
    void** staticFieldValue;
    map<string, int> ftp; // NameAndDescriptor 到 Field的偏移量的转换
    vector<int> byteGrad; // 字节的偏移量
    ConstantPool* pcp;

    ConstantPoolMetaType* GetConstantPoolItem(int pl_index);// {return pcp->GetConstantPoolItem(pl_index);}

    inline int GetByte() {return byteGrad[byteGrad.size()-1];}
    bool pClass::PutIndexStaticField(UInt index, char* source, int length) ;
    void* pClass::GetIndexStaticField(UInt index) ;
    void pClass::BuildStaticFields() ;

    bool ACC_PUBLIC;
    bool ACC_FINAL;
    bool ACC_SUPER;
    bool ACC_INTERFACE;
    bool ACC_ABSTRACT;
    bool ACC_SYNTHETIC;
    bool ACC_ANNOTATION;
    bool ACC_ENUM;

    UInt magic;
    UShort minor_version;
    UShort major_version;
    UShort this_class;
    UShort constant_pool_count;

    UShort field_count;
    UShort static_field_count;
    UShort method_count;

    pClass();
    pClass(class_attribute*);
    inline std::string getClassName() {return "this";}

    MethodEntry* GetStaticMethod(string nat);
    int MakeVirtualTable();
    int MakeStaticMethodTable();
    ~pClass(); 
    UBoolean is_loaded();
    void set_loaded();
};
#endif
