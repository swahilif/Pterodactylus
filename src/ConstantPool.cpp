#include "../include/runtime/ConstantPool.h"
#include <iostream>
using std::cout;
using std::endl;
/*
 * Please move from the exit rows if you are unwilling and unable to perform the necessary actions without injury
 */

//int ConstantPoolMetaType::GetInstanceType() {return instance_type;}

ConstantPoolRefType::ConstantPoolRefType(UShort _class_index, UShort _entry_index, int _instance_type) {
    instance_type = _instance_type; 
    class_index = _class_index;
    entry_index = _entry_index;
    nat = "";
}

NAT ConstantPoolRefType::GetNameAndType() {
    if (nat == "") {
        return ""; 
    } 
    return nat;
}

ConstantPoolValueType::ConstantPoolValueType(void* _value, int _instant_type) {
    instance_type = _instant_type;
    if (instance_type == CONS_FLOAT) 
        value = new float(*(float*)_value);
    else if (instance_type == CONS_DOUBLE) 
        value = new double(*(double*)_value);
    else if (instance_type == CONS_LONG)    
        value = new long(*(long*)_value);
    else if (instance_type == CONS_INTEGER) 
        value = new int(*(int*)_value);
    else if (instance_type == CONS_STRING)
        value = new std::string((char*)_value);
    else if (instance_type == CONS_UTF8)
        value = new NAT(*(NAT*)_value);
    else if (instance_type == CONS_CLASS)
        value = new UShort(*(UShort*)_value);
}

ConstantPoolRefType::~ConstantPoolRefType() {

}

ConstantPoolValueType::~ConstantPoolValueType() {
    delete value;
}

void* ConstantPoolValueType::GetValue() { return value; }

ConstantPoolNAT::ConstantPoolNAT(UInt _name_index, UInt _descriptor_index) {
    name_index = _name_index;
    instance_type = CONS_NAME_TYPE;
    descriptor_index = _descriptor_index;
    nat = "";
}

NAT ConstantPoolNAT::GetNameAndType() {
    return nat;
}

bool ConstantPool::ResolveCLS() {
    for (int k=1; k<constant_pool_count; k++) {
        if (cpm[k]->GetInstanceType() == CONS_CLASS) {
            auto tmp = reinterpret_cast<ConstantPoolValueType*>(cpm[k]);
            tmp->value = (string*) cpm[*(UShort*)tmp->value]->GetValue();
        }
    }
}

bool ConstantPool::ResolveNAT() {
    for (int k=1; k<constant_pool_count; k++) {
        if (cpm[k]->GetInstanceType() == CONS_NAME_TYPE) {
            auto tmp = reinterpret_cast<ConstantPoolNAT*>(cpm[k]);
            tmp->nat = *(string*)cpm[tmp->name_index]->GetValue() + ":" + *(string*)cpm[tmp->descriptor_index]->GetValue();
        }
    }
}

bool ConstantPool::ResolveRef() {
    for (int k=1; k<constant_pool_count; k++) {
        if (cpm[k]->GetInstanceType() == CONS_FIELD || cpm[k]->GetInstanceType() == CONS_METHOD || cpm[k]->GetInstanceType() == CONS_INTERFACE) {
            auto tmp = reinterpret_cast<ConstantPoolRefType*>(cpm[k]);
            tmp->nat = *(string*)cpm[tmp->class_index]->GetValue() + "." + cpm[tmp->entry_index]->GetNameAndType();
            //tmp->value = (string*) cpm[tmp->value]->GetValue();
        }
    }
}

ConstantPoolNAT::~ConstantPoolNAT() {
    
}

ConstantPool::ConstantPool(CONSTANT_METATYPE** pcm, UShort ConstantPoolCount) {
    constant_pool_count = ConstantPoolCount;
    cpm = new ConstantPoolMetaType*[constant_pool_count+1];
    cout << "came into here!" << endl;
    cout << "Constant pool count : " << ConstantPoolCount << endl;
    cout << pcm << endl;
    cout << pcm[1] << endl;
        
    for (int i = 1; i < ConstantPoolCount; i++) {
        auto instype = pcm[i]->get_instance_type();
        if (instype == CONS_METATYPE) throw "ConstantPool can't have any CONS_METATYPE entry!";
        if (instype == CONS_UTF8) {
            cpm[i] = new ConstantPoolValueType(pcm[i]->get_value(), instype); 
        } else if (instype == CONS_INTEGER || instype == CONS_FLOAT || instype == CONS_LONG || instype == CONS_DOUBLE || instype == CONS_STRING) {
            cpm[i] = new ConstantPoolValueType(pcm[i]->get_value(), instype);
        } else if (instype == CONS_CLASS) { // 类的描述符
            cpm[i] = new ConstantPoolValueType(pcm[i]->get_value(), instype);
        } else if (instype == CONS_FIELD || instype == CONS_METHOD || instype == CONS_INTERFACE) {
            std::pair<UShort, UShort>* pus2 = (std::pair<UShort, UShort>*)pcm[i]->get_binary_info();
            cpm[i] = new ConstantPoolRefType(pus2->first, pus2->second, instype);
        } else if (instype == CONS_NAME_TYPE) {
            std::pair<UShort, UShort>* pus2 = (std::pair<UShort, UShort>*)pcm[i]->get_binary_info();
            cpm[i] = new ConstantPoolNAT(pus2->first, pus2->second);
        }  
        cout << "ConstantPool["<<i<<"] is "<<instype << endl;
    }

    cout << "Over" << endl;
}

ConstantPool::~ConstantPool() {
    if (cpm != NULL) {
        for (int i=1; i<constant_pool_count; i++) 
            if (cpm[i] != NULL) delete cpm[i];
        delete[] cpm;
    }
}

ConstantPoolMetaType* ConstantPool::GetConstantPoolItem(int index) {
    assert(index >= 1 && index < constant_pool_count);
    return cpm[index];
}

int ConstantPoolRefType::resolved(ConstantPool* pcp) { // resolve the index form into string form 
    cout << "Just here1" << endl;
    auto* pci = pcp->GetConstantPoolItem(class_index);
    auto* pei = pcp->GetConstantPoolItem(entry_index);
    cout << pci << "><" << pei << endl;
    if (pci == NULL || pei == NULL)
        return 0;
    cout << pci->GetValue() << "><" << pei->GetNameAndType() << endl;
    if (pci->GetValue() == NULL || pei->GetValue() == NULL) // 无法获得字符串
        return 0;
    UInt utf8_index = *(UInt*)pci->GetValue();
    auto* pui = pcp->GetConstantPoolItem(utf8_index);
    if (pui == NULL || pui->GetValue() == NULL) return 0;
    this->nat = *(NAT*)pui->GetValue() + "." + pei->GetNameAndType();
    return 1;
}

void* ConstantPoolNAT::GetValue() {return NULL;}
void* ConstantPoolRefType::GetValue() {return NULL;}
int ConstantPoolValueType::resolved(ConstantPool*p){return 0;}

int ConstantPoolNAT::resolved(ConstantPool* pcp) {
    auto* pci = pcp->GetConstantPoolItem(name_index);
    auto* pei = pcp->GetConstantPoolItem(descriptor_index);

    if (pci == NULL || pei == NULL)
        return 0;
    if (pci->GetValue() == NULL || pei->GetValue() == NULL) // 无法获得字符串
        return 0;

    this->nat = *(NAT*)pci->GetValue() + ":" + *(NAT*)pei->GetValue();
    return 1;
}
