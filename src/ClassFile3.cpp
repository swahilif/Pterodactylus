#include "../include/runtime/Field.h"
#include "../include/types/ClassFile.h"
#include "../include/types/ClassLoader.h"

#include <iostream>
using std::cout;
using std::endl;
std::map<std::string, int> nameMap;
    
const int MAX_LOADED_CLASSNUM = 1000;
pClass* arrLoadedClass[MAX_LOADED_CLASSNUM];  

pClass::pClass(class_attribute* pcf):ACC_PUBLIC(pcf->ACC_PUBLIC), ACC_FINAL(pcf->ACC_FINAL), ACC_SUPER(pcf->ACC_SUPER), ACC_INTERFACE(pcf->ACC_INTERFACE), 
        ACC_ABSTRACT(pcf->ACC_ABSTRACT), ACC_SYNTHETIC(pcf->ACC_SYNTHETIC), ACC_ANNOTATION(pcf->ACC_ANNOTATION), ACC_ENUM(pcf->ACC_ENUM) {

            std::string fatherClassName = pcf->get_class_name(pcf->parent_index);

            pFatherClass = ClassLoader::findLoadedClass(fatherClassName); // 尝试获取父类 
            cout << fatherClassName << " REE " << pFatherClass << endl;

            if ((fatherClassName != "java/lang/Object") && pFatherClass == NULL) {
                ClassLoader::LoadClass(fatherClassName, true);
                pFatherClass = ClassLoader::findLoadedClass(fatherClassName);
            }
            assert((fatherClassName == "java/lang/Object") || (pFatherClass != NULL)); // 父类必须存在

            //if (fatherClassName != "java/lang/Object" && !pFatherClass->is_loaded()) {
            if (fatherClassName != "java/lang/Object" && !pFatherClass->is_loaded()) {
                //if (!LoadClass(pFatherClass))
                    throw "Can't load father class into runtime environment.";
                // Load Father Class
            }

            magic = pcf->magic;
            minor_version = pcf->minor_version;
            major_version = pcf->major_version;
            //this_class = pcf->this_class;
            constant_pool_count = pcf->constant_pool_count;

            field_count = pcf->field_count;
            method_count = pcf->method_count;
            constant_pool_count = pcf->constant_pool_count;

            arrMethod = new MethodEntry*[method_count];
            //arrField = new FieldEntry*[field_count];
            printf("minv %d  majv %d   cpc %d\n", minor_version, major_version, constant_pool_count);
            printf("methodcount %d\n", method_count);

            for (auto k = method_count - 1; k >= 0; k--) {
                METHODINFO *pmi = pcf->method_info[k];
                arrMethod[k] = new MethodEntry(pmi, pcf);
                cout << "happy!" << k << endl;
            }
            cout << "New" << field_count << endl;

            static_field_count = 0;

            for (auto k = field_count - 1; k >= 0; k--) {
                FIELDINFO *pfi = pcf->field_info[k];
                if (pcf->ACC_STATIC)
                    static_field_count++;
            }

            arrField = new FieldEntry*[field_count - static_field_count];
            staticField = new FieldEntry*[staticField];
            for (auto k = 0, idx = 0, sidx = 0; k < field_count; k++) {
                FIELDINFO *pfi = pcf->field_info[k];
                if (!pcf->ACC_STATIC) 
                    arrField[idx++] = new FieldEntry(pfi, pcf);
                else
                    staticField[sidx++] = new FieldEntry(pfi, pcf);
                //cout << "singout!" << k << endl;
            }           

            BuildStaticFields();

            field_count -= staticField;
            MakeFieldTable(ftp, this, field_count);

            cout << "Have made field table." << endl;

            // Init the f**ked constant pool
            this->pcp = new ConstantPool(pcf->constant_pool, this->constant_pool_count);
            this->pcp->ResolveCLS();
            this->pcp->ResolveNAT();
            this->pcp->ResolveRef();

            // Init the virtual function table
            //this->vtp = new VirtualTable(this);
            this->MakeVirtualTable();
            this->MakeStaticMethodTable();
            //this->smtp = new StaticMethodTable(this);

            // Load This Class.
            //if (!ClassLoader::LoadClass(pcf)) {
            //    throw "Can't load this class into runtime environment.";
            //}
    }

int pClass::MakeVirtualTable() {
    vtp = new VirtualTable(this);
    return vtp == NULL;
}

int pClass::MakeStaticMethodTable() {
    smtp = new StaticMethodTable(this);
    return smtp == NULL;
}

// 构建静态字段表
void pClass::BuildStaticFields() {
    staticFieldValue = new void*[static_field_count];
    for (int k = 0; k < static_field_count; k++) {
        staticFieldValue[k] = new char[staticField[k]->GetByte()];
    }
}

void* pClass::GetIndexStaticField(UInt index) {
    return staticFieldValue[index];
}

bool pClass::PutIndexStaticField(UInt index, char* source, int length) {
    assert(index >= 0 && index < static_field_count);
    memcpy(staticFieldValue[index], source, length);
    return true;
}

ConstantPoolMetaType* pClass::GetConstantPoolItem(int pl_index) {return pcp->GetConstantPoolItem(pl_index);}
pClass::~pClass() {
    if (vtp != NULL) delete vtp;
    if (smtp != NULL) delete smtp;
    if (arrMethod != NULL) { 
        for (auto k = method_count - 1; k >= 0; k--) 
            if (arrMethod[k] != NULL) delete arrMethod[k];
        delete[] arrMethod;
    } 
    if (arrField != NULL) {
        for (auto k = field_count - 1; k >= 0; k--)
            if (arrField[k] != NULL) delete arrField[k];
        delete[] arrField;
    }
}

MethodEntry* pClass::GetStaticMethod(string nat) {
    pClass* pkl = this;
    while (pkl != NULL) {
        if (this->smtp->nameMap.count(nat)) 
            return this->smtp->entryList[nameMap[nat]];
        pkl = pkl->pFatherClass;
    } 
    return NULL; // 返回NULL，使得Resolve从父类继续解析
}

pClass::pClass() {

}

UBoolean pClass::is_loaded() {
    return this->is_loaded_;
}

void pClass::set_loaded() {
    this->is_loaded_ = true;
}