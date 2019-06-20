#include "MethodPool.h"
#include <map>
#include <string>
using std::string;
using std::map;

// [ClassName] StaticMethodTable
// 处理所有静态函数和私有函数
#ifndef STATIC_METHOD_TABLE
#define STATIC_METHOD_TABLE
using MapContainerType = map<string, int>;
using ContainerType = vector<MethodEntry*>;

class StaticMethodTable {
    MapContainerType nameMap;
    ContainerType entryList;
public:
    friend class pClass;
    StaticMethodTable(pClass* pkl) ;/*{
        for (int i=0; i<pkl->method_count; i++) {
            auto *it = pkl->arrMethod[i];
            if (it->GetAccessFlags("ACC_STATIC") || it->GetAccessFlags("ACC_PRIVATE")) {
                entryList.append();
                nameMap[it->GetNameAndDescriptor()] = entryList.length() - 1;
            }
        }
    }*/

    MethodEntry* GetStaticMethodEntry(string nat) ;/*{
        if (nameMap.count(nat)) 
            return entryList[nameMap[nat]];
        return NULL; // 返回NULL，使得Resolve从父类继续解析
    }*/
};
#endif
