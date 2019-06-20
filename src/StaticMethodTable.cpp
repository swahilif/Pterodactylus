#include "../include/types/ClassFile.h"

StaticMethodTable::StaticMethodTable(pClass* pkl) {
    for (int i=0; i<pkl->method_count; i++) {
        auto *it = pkl->arrMethod[i];
        if (it->GetAccessFlags("ACC_STATIC") || it->GetAccessFlags("ACC_PRIVATE")) {
            entryList.push_back(it);
            nameMap[it->GetNameAndDescriptor()] = entryList.size() - 1;
        }
    }
    //cout << entryList[0] << endl;
}

MethodEntry* StaticMethodTable::GetStaticMethodEntry(string nat) {
    if (nameMap.count(nat)){
        return entryList[nameMap[nat]];
    }
    return NULL; // 返回NULL，使得Resolve从父类继续解析
}
